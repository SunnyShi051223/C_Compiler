#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// VM 指令类型
enum class CommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    C_UNKNOWN
};

// VM 指令结构
struct VMCommand {
    CommandType type;
    string arg1;
    int arg2;
    string rawLine;
};

// VMTranslator 类
class VMTranslator {
private:
    string currentFile;
    int labelCounter;
    string currentFunction;

    // 去除字符串首尾空白
    string trim(const string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == string::npos) return "";
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

    // 去除注释
    string removeComments(const string& line) {
        size_t commentPos = line.find("//");
        if (commentPos != string::npos) {
            return line.substr(0, commentPos);
        }
        return line;
    }

    // 解析单行 VM 指令
    VMCommand parseLine(const string& line) {
        VMCommand cmd;
        cmd.type = CommandType::C_UNKNOWN;
        cmd.arg2 = 0;

        string trimmed = trim(removeComments(line));
        if (trimmed.empty()) {
            return cmd;
        }

        istringstream iss(trimmed);
        string token;
        iss >> token;

        if (token == "add" || token == "sub" || token == "neg" ||
            token == "eq" || token == "gt" || token == "lt" ||
            token == "and" || token == "or" || token == "not") {
            cmd.type = CommandType::C_ARITHMETIC;
            cmd.arg1 = token;
        }
        else if (token == "push") {
            cmd.type = CommandType::C_PUSH;
            iss >> cmd.arg1 >> cmd.arg2;
        }
        else if (token == "pop") {
            cmd.type = CommandType::C_POP;
            iss >> cmd.arg1 >> cmd.arg2;
        }
        else if (token == "label") {
            cmd.type = CommandType::C_LABEL;
            iss >> cmd.arg1;
        }
        else if (token == "goto") {
            cmd.type = CommandType::C_GOTO;
            iss >> cmd.arg1;
        }
        else if (token == "if-goto") {
            cmd.type = CommandType::C_IF;
            iss >> cmd.arg1;
        }
        else if (token == "function") {
            cmd.type = CommandType::C_FUNCTION;
            iss >> cmd.arg1 >> cmd.arg2;
        }
        else if (token == "call") {
            cmd.type = CommandType::C_CALL;
            iss >> cmd.arg1 >> cmd.arg2;
        }
        else if (token == "return") {
            cmd.type = CommandType::C_RETURN;
        }

        cmd.rawLine = trimmed;
        return cmd;
    }

    // 生成唯一标签
    string generateLabel(const string& prefix) {
        return prefix + "_" + to_string(labelCounter++);
    }

    // 生成算术/逻辑指令的汇编代码
    string translateArithmetic(const string& command) {
        stringstream ss;

        if (command == "add") {
            ss << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M+D\n";
        }
        else if (command == "sub") {
            ss << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n";
        }
        else if (command == "neg") {
            ss << "@SP\nA=M-1\nM=-M\n";
        }
        else if (command == "and") {
            ss << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M&D\n";
        }
        else if (command == "or") {
            ss << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M|D\n";
        }
        else if (command == "not") {
            ss << "@SP\nA=M-1\nM=!M\n";
        }
        else if (command == "eq" || command == "gt" || command == "lt") {
            string trueLabel = generateLabel("TRUE");
            string endLabel = generateLabel("END");

            ss << "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n";
            ss << "@" << trueLabel << "\n";

            if (command == "eq") {
                ss << "D;JEQ\n";
            } else if (command == "gt") {
                ss << "D;JGT\n";
            } else {
                ss << "D;JLT\n";
            }

            ss << "@SP\nA=M-1\nM=0\n";
            ss << "@" << endLabel << "\n0;JMP\n";
            ss << "(" << trueLabel << ")\n";
            ss << "@SP\nA=M-1\nM=-1\n";
            ss << "(" << endLabel << ")\n";
        }

        return ss.str();
    }

    // 生成 push 指令的汇编代码
    string translatePush(const string& segment, int index) {
        stringstream ss;

        if (segment == "constant") {
            ss << "@" << index << "\nD=A\n";
        }
        else if (segment == "local") {
            ss << "@LCL\nD=M\n@" << index << "\nA=D+A\nD=M\n";
        }
        else if (segment == "argument") {
            ss << "@ARG\nD=M\n@" << index << "\nA=D+A\nD=M\n";
        }
        else if (segment == "this") {
            ss << "@THIS\nD=M\n@" << index << "\nA=D+A\nD=M\n";
        }
        else if (segment == "that") {
            ss << "@THAT\nD=M\n@" << index << "\nA=D+A\nD=M\n";
        }
        else if (segment == "temp") {
            ss << "@R" << (5 + index) << "\nD=M\n";
        }
        else if (segment == "pointer") {
            if (index == 0) {
                ss << "@THIS\nD=M\n";
            } else {
                ss << "@THAT\nD=M\n";
            }
        }
        else if (segment == "static") {
            string label = currentFile + "." + to_string(index);
            ss << "@" << label << "\nD=M\n";
        }

        // 压入栈
        ss << "@SP\nA=M\nM=D\n@SP\nM=M+1\n";

        return ss.str();
    }

    // 生成 pop 指令的汇编代码
    string translatePop(const string& segment, int index) {
        stringstream ss;

        if (segment == "local" || segment == "argument" ||
            segment == "this" || segment == "that") {
            string baseReg;
            if (segment == "local") baseReg = "LCL";
            else if (segment == "argument") baseReg = "ARG";
            else if (segment == "this") baseReg = "THIS";
            else baseReg = "THAT";

            ss << "@" << baseReg << "\nD=M\n@" << index << "\nD=D+A\n@R13\nM=D\n";
            ss << "@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
        }
        else if (segment == "temp") {
            ss << "@SP\nAM=M-1\nD=M\n@R" << (5 + index) << "\nM=D\n";
        }
        else if (segment == "pointer") {
            ss << "@SP\nAM=M-1\nD=M\n";
            if (index == 0) {
                ss << "@THIS\nM=D\n";
            } else {
                ss << "@THAT\nM=D\n";
            }
        }
        else if (segment == "static") {
            string label = currentFile + "." + to_string(index);
            ss << "@SP\nAM=M-1\nD=M\n@" << label << "\nM=D\n";
        }

        return ss.str();
    }

    // 生成 label 指令的汇编代码
    string translateLabel(const string& label) {
        string fullLabel = currentFunction.empty() ? label : currentFunction + "$" + label;
        return "(" + fullLabel + ")\n";
    }

    // 生成 goto 指令的汇编代码
    string translateGoto(const string& label) {
        string fullLabel = currentFunction.empty() ? label : currentFunction + "$" + label;
        return "@" + fullLabel + "\n0;JMP\n";
    }

    // 生成 if-goto 指令的汇编代码
    string translateIf(const string& label) {
        string fullLabel = currentFunction.empty() ? label : currentFunction + "$" + label;
        return "@SP\nAM=M-1\nD=M\n@" + fullLabel + "\nD;JNE\n";
    }

    // 生成 function 指令的汇编代码
    string translateFunction(const string& functionName, int nLocals) {
        stringstream ss;
        currentFunction = functionName;

        ss << "(" << functionName << ")\n";

        // 初始化局部变量为 0
        for (int i = 0; i < nLocals; i++) {
            ss << "@SP\nA=M\nM=0\n@SP\nM=M+1\n";
        }

        return ss.str();
    }

    // 生成 call 指令的汇编代码
    string translateCall(const string& functionName, int nArgs) {
        stringstream ss;
        string returnLabel = generateLabel("RETURN");

        // 保存返回地址
        ss << "@" << returnLabel << "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";

        // 保存调用帧
        ss << "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        ss << "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        ss << "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        ss << "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";

        // 重置 ARG = SP - 5 - nArgs
        ss << "@SP\nD=M\n@5\nD=D-A\n@" << nArgs << "\nD=D-A\n@ARG\nM=D\n";

        // 重置 LCL = SP
        ss << "@SP\nD=M\n@LCL\nM=D\n";

        // 跳转到函数
        ss << "@" << functionName << "\n0;JMP\n";

        // 返回标签
        ss << "(" << returnLabel << ")\n";

        return ss.str();
    }

    // 生成 return 指令的汇编代码
    string translateReturn() {
        stringstream ss;

        // 保存 LCL 到临时寄存器 (endFrame)
        ss << "@LCL\nD=M\n@R13\nM=D\n";

        // 计算返回地址: retAddr = *(endFrame - 5)
        ss << "@5\nA=D-A\nD=M\n@R14\nM=D\n";

        // 将返回值放到 ARG[0]
        ss << "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n";

        // 恢复 SP = ARG + 1
        ss << "@ARG\nD=M+1\n@SP\nM=D\n";

        // 恢复调用帧（注意顺序：THAT, THIS, ARG, LCL）
        // THAT = *(endFrame - 1)
        ss << "@R13\nAM=M-1\nD=M\n@THAT\nM=D\n";
        // THIS = *(endFrame - 2)
        ss << "@R13\nAM=M-1\nD=M\n@THIS\nM=D\n";
        // ARG = *(endFrame - 3)
        ss << "@R13\nAM=M-1\nD=M\n@ARG\nM=D\n";
        // LCL = *(endFrame - 4)
        ss << "@R13\nAM=M-1\nD=M\n@LCL\nM=D\n";

        // 跳转到返回地址
        ss << "@R14\nA=M\n0;JMP\n";

        return ss.str();
    }

    // 生成初始化代码
    string translateInit() {
        stringstream ss;

        // 设置 SP=256
        ss << "@256\nD=A\n@SP\nM=D\n";

        // 调用 Main.main（而不是 Sys.init，因为我们的程序没有 Sys.init）
        ss << translateCall("Main.main", 0);

        return ss.str();
    }

public:
    VMTranslator() : labelCounter(0) {}

    // 翻译单个 .vm 文件
    string translateFile(const string& filename, bool generateInit = false) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file " << filename << endl;
            return "";
        }

        // 提取文件名（不含路径和扩展号）
        size_t lastSlash = filename.find_last_of("/\\");
        size_t lastDot = filename.find_last_of('.');
        if (lastSlash == string::npos) lastSlash = -1;
        currentFile = filename.substr(lastSlash + 1, lastDot - lastSlash - 1);

        stringstream output;
        labelCounter = 0;
        currentFunction = "";

        // 生成初始化代码（可选）
        if (generateInit) {
            output << translateInit();
        }

        // 逐行翻译
        string line;
        while (getline(file, line)) {
            VMCommand cmd = parseLine(line);

            if (cmd.type == CommandType::C_UNKNOWN) {
                continue;
            }

            // 添加原始代码作为注释
            output << "// " << cmd.rawLine << "\n";

            switch (cmd.type) {
                case CommandType::C_ARITHMETIC:
                    output << translateArithmetic(cmd.arg1);
                    break;
                case CommandType::C_PUSH:
                    output << translatePush(cmd.arg1, cmd.arg2);
                    break;
                case CommandType::C_POP:
                    output << translatePop(cmd.arg1, cmd.arg2);
                    break;
                case CommandType::C_LABEL:
                    output << translateLabel(cmd.arg1);
                    break;
                case CommandType::C_GOTO:
                    output << translateGoto(cmd.arg1);
                    break;
                case CommandType::C_IF:
                    output << translateIf(cmd.arg1);
                    break;
                case CommandType::C_FUNCTION:
                    output << translateFunction(cmd.arg1, cmd.arg2);
                    break;
                case CommandType::C_CALL:
                    output << translateCall(cmd.arg1, cmd.arg2);
                    break;
                case CommandType::C_RETURN:
                    output << translateReturn();
                    break;
                default:
                    break;
            }

            output << "\n";
        }

        file.close();
        return output.str();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: VMTranslator <input.vm> [output.asm]" << endl;
        cout << "       VMTranslator <input1.vm> <input2.vm> ... [output.asm]" << endl;
        return 1;
    }

    // 解析参数
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

    // 如果没有指定输出文件，使用第一个输入文件名
    if (outputFile.empty() && !inputFiles.empty()) {
        outputFile = inputFiles[0].substr(0, inputFiles[0].size() - 3) + ".asm";
    }

    // 翻译所有输入文件
    VMTranslator translator;
    stringstream finalOutput;

    // 总是生成初始化代码（设置 SP=256 并调用 Sys.init 或 Main.main）
    bool generateInit = true;

    for (size_t i = 0; i < inputFiles.size(); i++) {
        string content = translator.translateFile(inputFiles[i], (i == 0 && generateInit));
        finalOutput << content;
    }

    // 写入输出文件
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
