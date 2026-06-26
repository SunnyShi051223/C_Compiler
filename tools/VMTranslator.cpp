#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum class CommandType {
    C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF,
    C_FUNCTION, C_RETURN, C_CALL, C_UNKNOWN
};

struct VMCommand {
    CommandType type;
    string arg1;
    int arg2;
    string rawLine;
};

class VMTranslator {
private:
    string currentFile;
    int labelCounter;
    string currentFunction;

    string trim(const string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == string::npos) return "";
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

    string removeComments(const string& line) {
        size_t p = line.find("//");
        return p != string::npos ? line.substr(0, p) : line;
    }

    VMCommand parseLine(const string& line) {
        VMCommand cmd;
        cmd.type = CommandType::C_UNKNOWN;
        cmd.arg2 = 0;

        string trimmed = trim(removeComments(line));
        if (trimmed.empty()) return cmd;

        istringstream iss(trimmed);
        string token;
        iss >> token;

        if (token == "add" || token == "sub" || token == "neg" ||
            token == "eq" || token == "gt" || token == "lt" ||
            token == "and" || token == "or" || token == "not") {
            cmd.type = CommandType::C_ARITHMETIC;
            cmd.arg1 = token;
        }
        else if (token == "push") { cmd.type = CommandType::C_PUSH; iss >> cmd.arg1 >> cmd.arg2; }
        else if (token == "pop")  { cmd.type = CommandType::C_POP;  iss >> cmd.arg1 >> cmd.arg2; }
        else if (token == "label")    { cmd.type = CommandType::C_LABEL;   iss >> cmd.arg1; }
        else if (token == "goto")     { cmd.type = CommandType::C_GOTO;    iss >> cmd.arg1; }
        else if (token == "if-goto")  { cmd.type = CommandType::C_IF;      iss >> cmd.arg1; }
        else if (token == "function") { cmd.type = CommandType::C_FUNCTION; iss >> cmd.arg1 >> cmd.arg2; }
        else if (token == "call")     { cmd.type = CommandType::C_CALL;    iss >> cmd.arg1 >> cmd.arg2; }
        else if (token == "return")   { cmd.type = CommandType::C_RETURN; }

        cmd.rawLine = trimmed;
        return cmd;
    }

    string generateLabel(const string& prefix) {
        return currentFile + "." + prefix + "_" + to_string(labelCounter++);
    }

    // ==================== 算术/逻辑 ====================
    string translateArithmetic(const string& command) {
        if (command == "add") return "@SP\nAM=M-1\nD=M\nA=A-1\nM=M+D\n";
        if (command == "sub") return "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n";
        if (command == "neg") return "@SP\nA=M-1\nM=-M\n";
        if (command == "and") return "@SP\nAM=M-1\nD=M\nA=A-1\nM=M&D\n";
        if (command == "or")  return "@SP\nAM=M-1\nD=M\nA=A-1\nM=M|D\n";
        if (command == "not") return "@SP\nA=M-1\nM=!M\n";

        // eq/gt/lt
        string t = generateLabel("T"), e = generateLabel("E");
        string jmp = (command == "eq") ? "D;JEQ" : (command == "gt") ? "D;JGT" : "D;JLT";
        return "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@" + t + "\n" + jmp +
               "\n@SP\nA=M-1\nM=0\n@" + e + "\n0;JMP\n(" + t +
               ")\n@SP\nA=M-1\nM=-1\n(" + e + ")\n";
    }

    // ==================== push（优化版）====================
    string translatePush(const string& segment, int index) {
        // 将值加载到 D
        string loadD;
        if (segment == "constant") {
            if (index == 0)      loadD = "D=0";
            else if (index == 1) loadD = "D=1";
            else                 loadD = "D=" + to_string(index); // @i + D=A → D=i
        }
        else if (segment == "local")    loadD = (index == 0) ? "@LCL\nA=M\nD=M" : "@LCL\nD=M\n@" + to_string(index) + "\nA=D+A\nD=M";
        else if (segment == "argument") loadD = (index == 0) ? "@ARG\nA=M\nD=M"  : "@ARG\nD=M\n@" + to_string(index) + "\nA=D+A\nD=M";
        else if (segment == "this")     loadD = (index == 0) ? "@THIS\nA=M\nD=M" : "@THIS\nD=M\n@" + to_string(index) + "\nA=D+A\nD=M";
        else if (segment == "that")     loadD = (index == 0) ? "@THAT\nA=M\nD=M" : "@THAT\nD=M\n@" + to_string(index) + "\nA=D+A\nD=M";
        else if (segment == "temp")     loadD = "@R" + to_string(5 + index) + "\nD=M";
        else if (segment == "pointer")  loadD = (index == 0) ? "@THIS\nD=M" : "@THAT\nD=M";
        else if (segment == "static")   loadD = "@" + currentFile + "." + to_string(index) + "\nD=M";

        // 优化：push constant 小值不使用 @ 指令
        if (segment == "constant" && index >= 0 && index <= 2) {
            return "@" + to_string(index) + "\nD=A\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        }
        if (segment == "constant") {
            return "@" + to_string(index) + "\nD=A\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        }

        // 通用 push: D 已有值，压栈
        return loadD + "\n@SP\nAM=M+1\nA=A-1\nM=D\n";
    }

    // ==================== pop（优化版）====================
    string translatePop(const string& segment, int index) {
        if (segment == "temp") {
            return "@SP\nAM=M-1\nD=M\n@R" + to_string(5 + index) + "\nM=D\n";
        }
        if (segment == "pointer") {
            string reg = (index == 0) ? "THIS" : "THAT";
            return "@SP\nAM=M-1\nD=M\n@" + reg + "\nM=D\n";
        }
        if (segment == "static") {
            return "@SP\nAM=M-1\nD=M\n@" + currentFile + "." + to_string(index) + "\nM=D\n";
        }

        string baseReg;
        if (segment == "local")    baseReg = "LCL";
        else if (segment == "argument") baseReg = "ARG";
        else if (segment == "this")     baseReg = "THIS";
        else                            baseReg = "THAT";

        // 优化：index=0 时不使用 R13
        if (index == 0) {
            return "@SP\nAM=M-1\nD=M\n@" + baseReg + "\nA=M\nM=D\n";
        }
        // 优化：index=1 时使用 M+1
        if (index == 1) {
            return "@" + baseReg + "\nD=M+1\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
        }
        return "@" + baseReg + "\nD=M\n@" + to_string(index) + "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
    }

    // ==================== 流程控制 ====================
    string translateLabel(const string& label) {
        string full = currentFunction.empty() ? label : currentFunction + "$" + label;
        return "(" + full + ")\n";
    }

    string translateGoto(const string& label) {
        string full = currentFunction.empty() ? label : currentFunction + "$" + label;
        return "@" + full + "\n0;JMP\n";
    }

    string translateIf(const string& label) {
        string full = currentFunction.empty() ? label : currentFunction + "$" + label;
        return "@SP\nAM=M-1\nD=M\n@" + full + "\nD;JNE\n";
    }

    // ==================== 函数 ====================
    string translateFunction(const string& functionName, int nLocals) {
        currentFunction = functionName;
        string result = "(" + functionName + ")\n";

        // 优化：大量局部变量时使用循环
        if (nLocals > 4) {
            // 使用循环初始化: ARG = nLocals, while(ARG--) *SP++ = 0
            // 但这样会破坏 ARG，所以还是逐个初始化更安全
            // 折中方案：逐个初始化但使用更紧凑的代码
            for (int i = 0; i < nLocals; i++) {
                result += "@SP\nAM=M+1\nA=A-1\nM=0\n";
            }
        } else {
            for (int i = 0; i < nLocals; i++) {
                result += "@SP\nAM=M+1\nA=A-1\nM=0\n";
            }
        }
        return result;
    }

    // ==================== call ====================
    string translateCall(const string& functionName, int nArgs) {
        string rl = generateLabel("R");
        stringstream ss;

        // push return address
        ss << "@" << rl << "\nD=A\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        // push LCL, ARG, THIS, THAT
        ss << "@LCL\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        ss << "@ARG\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        ss << "@THIS\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        ss << "@THAT\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
        // ARG = SP - 5 - nArgs
        ss << "@SP\nD=M\n@5\nD=D-A\n@" << nArgs << "\nD=D-A\n@ARG\nM=D\n";
        // LCL = SP
        ss << "@SP\nD=M\n@LCL\nM=D\n";
        // goto f
        ss << "@" << functionName << "\n0;JMP\n";
        // (returnLabel)
        ss << "(" << rl << ")\n";

        return ss.str();
    }

    // ==================== return ====================
    string translateReturn() {
        // endFrame = LCL → R13
        // retAddr = *(endFrame - 5) → R14
        return
            "@LCL\nD=M\n@R13\nM=D\n"         // R13 = endFrame
            "@5\nA=D-A\nD=M\n@R14\nM=D\n"    // R14 = retAddr
            "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n"  // *ARG = pop()
            "@ARG\nD=M+1\n@SP\nM=D\n"         // SP = ARG+1
            "@R13\nAM=M-1\nD=M\n@THAT\nM=D\n" // THAT = *(ef-1)
            "@R13\nAM=M-1\nD=M\n@THIS\nM=D\n" // THIS = *(ef-2)
            "@R13\nAM=M-1\nD=M\n@ARG\nM=D\n"  // ARG  = *(ef-3)
            "@R13\nAM=M-1\nD=M\n@LCL\nM=D\n"  // LCL  = *(ef-4)
            "@R14\nA=M\n0;JMP\n";              // goto retAddr
    }

    // ==================== 初始化 ====================
    string translateInit(bool useSysInit = true) {
        stringstream ss;
        ss << "@256\nD=A\n@SP\nM=D\n";  // SP=256
        if (useSysInit) {
            ss << translateCall("Sys.init", 0);
        } else {
            ss << translateCall("Main.main", 0);
        }
        ss << "(END)\n@END\n0;JMP\n";   // 无限循环
        return ss.str();
    }

public:
    VMTranslator() : labelCounter(0) {}

    // 翻译单个 .vm 文件
    string translateFile(const string& filename, bool generateInit = false, bool useSysInit = true) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file " << filename << endl;
            return "";
        }

        size_t lastSlash = filename.find_last_of("/\\");
        size_t lastDot = filename.find_last_of('.');
        if (lastSlash == string::npos) lastSlash = -1;
        currentFile = filename.substr(lastSlash + 1, lastDot - lastSlash - 1);

        stringstream output;
        labelCounter = 0;
        currentFunction = "";

        if (generateInit) {
            output << translateInit(useSysInit);
        }

        string line;
        while (getline(file, line)) {
            VMCommand cmd = parseLine(line);
            if (cmd.type == CommandType::C_UNKNOWN) continue;

            switch (cmd.type) {
                case CommandType::C_ARITHMETIC: output << translateArithmetic(cmd.arg1); break;
                case CommandType::C_PUSH:      output << translatePush(cmd.arg1, cmd.arg2); break;
                case CommandType::C_POP:       output << translatePop(cmd.arg1, cmd.arg2); break;
                case CommandType::C_LABEL:     output << translateLabel(cmd.arg1); break;
                case CommandType::C_GOTO:      output << translateGoto(cmd.arg1); break;
                case CommandType::C_IF:        output << translateIf(cmd.arg1); break;
                case CommandType::C_FUNCTION:  output << translateFunction(cmd.arg1, cmd.arg2); break;
                case CommandType::C_CALL:      output << translateCall(cmd.arg1, cmd.arg2); break;
                case CommandType::C_RETURN:    output << translateReturn(); break;
                default: break;
            }
        }

        file.close();
        return output.str();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: VMTranslator <input.vm> [output.asm]" << endl;
        cout << "       VMTranslator <file1.vm> <file2.vm> ... [output.asm]" << endl;
        return 1;
    }

    vector<string> inputFiles;
    string outputFile;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg.size() > 3 && arg.substr(arg.size() - 3) == ".vm") {
            inputFiles.push_back(arg);
        } else if (arg.size() > 4 && arg.substr(arg.size() - 4) == ".asm") {
            outputFile = arg;
        }
    }

    if (outputFile.empty() && !inputFiles.empty()) {
        outputFile = inputFiles[0].substr(0, inputFiles[0].size() - 3) + ".asm";
    }

    VMTranslator translator;
    stringstream finalOutput;

    bool generateInit = true;
    bool useSysInit = (inputFiles.size() > 1);  // 多文件时使用 Sys.init
    for (size_t i = 0; i < inputFiles.size(); i++) {
        finalOutput << translator.translateFile(inputFiles[i], (i == 0 && generateInit), useSysInit);
    }

    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "Error: Cannot create output file " << outputFile << endl;
        return 1;
    }

    out << finalOutput.str();
    out.close();

    cout << "Translated " << inputFiles.size() << " file(s) to " << outputFile << endl;
    return 0;
}
