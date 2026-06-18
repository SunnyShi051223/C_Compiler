#include "../include/SemanticAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using std::string;
using std::vector;
using std::ofstream;
using std::cerr;
using std::endl;
using std::to_string;
using std::map;

SemanticAnalyzer::SemanticAnalyzer() : tempCount_(0), labelCount_(0), varCount_(0) {}

string SemanticAnalyzer::newTemp() {
    return "t" + to_string(tempCount_++);
}

string SemanticAnalyzer::newLabel() {
    return "L" + to_string(labelCount_++);
}

void SemanticAnalyzer::declareVariable(const string& name, const string& type) {
    if (symbolTable_.find(name) == symbolTable_.end()) {
        symbolTable_[name] = {name, type, varCount_++, false};
    }
}

bool SemanticAnalyzer::isDeclared(const string& name) const {
    return symbolTable_.find(name) != symbolTable_.end();
}

string SemanticAnalyzer::getType(const string& name) const {
    auto it = symbolTable_.find(name);
    return it != symbolTable_.end() ? it->second.type : "";
}

int SemanticAnalyzer::getOffset(const string& name) const {
    auto it = symbolTable_.find(name);
    return it != symbolTable_.end() ? it->second.offset : -1;
}

int SemanticAnalyzer::getVariableCount() const {
    return varCount_;
}

// --- 结构体类型表 ---
void SemanticAnalyzer::defineStruct(const string& name) {
    if (structTable_.find(name) == structTable_.end()) {
        structTable_[name] = {name, {}, {}};
    }
}

void SemanticAnalyzer::addStructMember(const string& structName, const string& memberType, const string& memberName) {
    auto it = structTable_.find(structName);
    if (it != structTable_.end()) {
        it->second.memberNames.push_back(memberName);
        it->second.memberTypes.push_back(memberType);
    }
}

bool SemanticAnalyzer::isStructDefined(const string& name) const {
    return structTable_.find(name) != structTable_.end();
}

bool SemanticAnalyzer::isStructMember(const string& structName, const string& memberName) const {
    auto it = structTable_.find(structName);
    if (it == structTable_.end()) return false;
    for (const auto& m : it->second.memberNames) {
        if (m == memberName) return true;
    }
    return false;
}

// --- 语义检查 ---
void SemanticAnalyzer::checkStructDefined(const string& name, int line) {
    if (!isStructDefined(name)) {
        std::cerr << "Semantic Error at Line " << line
                  << ": struct type '" << name << "' is not defined\n";
    }
}

void SemanticAnalyzer::checkIsStruct(const string& varName, int line) {
    if (!isDeclared(varName)) return;
    string type = getType(varName);
    if (!isStructDefined(type)) {
        std::cerr << "Semantic Error at Line " << line
                  << ": variable '" << varName << "' is not a struct type\n";
    }
}

void SemanticAnalyzer::checkMemberAccess(const string& varName, const string& member, int line) {
    if (!isDeclared(varName)) return;
    string type = getType(varName);
    if (!isStructDefined(type)) {
        std::cerr << "Semantic Error at Line " << line
                  << ": variable '" << varName << "' is not a struct type\n";
    } else if (!isStructMember(type, member)) {
        std::cerr << "Semantic Error at Line " << line
                  << ": struct '" << type << "' has no member '" << member << "'\n";
    }
}

// --- 类型检查 ---
void SemanticAnalyzer::checkAssignment(const string& lhs, const string& rhs, int line) {
    // 如果任一变量未声明，跳过检查
    if (!isDeclared(lhs) || !isDeclared(rhs)) return;

    string lhsType = getType(lhs);
    string rhsType = getType(rhs);

    // 类型相同，OK
    if (lhsType == rhsType) return;

    // int ← char : 不允许（精度损失）
    if (lhsType == "int" && rhsType == "char") {
        std::cerr << "Type Warning at Line " << line
                  << ": assigning char to int ('" << rhs << "' to '" << lhs << "')\n";
    }
    // char ← int : 不允许（截断）
    else if (lhsType == "char" && rhsType == "int") {
        std::cerr << "Type Warning at Line " << line
                  << ": assigning int to char ('" << rhs << "' to '" << lhs << "')\n";
    }
    // double ← int 或 int ← double : 允许（隐式转换），但给警告
    else if ((lhsType == "double" && rhsType == "int") ||
             (lhsType == "int" && rhsType == "double")) {
        std::cerr << "Type Warning at Line " << line
                  << ": implicit conversion between int and double\n";
    }
    // struct 类型不匹配
    else if (lhsType != rhsType) {
        std::cerr << "Type Error at Line " << line
                  << ": incompatible types ('" << rhsType << "' to '" << lhsType << "')\n";
    }
}

void SemanticAnalyzer::checkDeclared(const string& name, int line) {
    if (!isDeclared(name)) {
        std::cerr << "Semantic Error at Line " << line
                  << ": variable '" << name << "' is not declared\n";
    }
}

int SemanticAnalyzer::emit(const string& op, const string& arg1,
                           const string& arg2, const string& result) {
    Quadruple q;
    q.id = (int)quads_.size();
    q.op = op;
    q.arg1 = arg1;
    q.arg2 = arg2;
    q.result = result;
    quads_.push_back(q);
    return q.id;
}

const vector<Quadruple>& SemanticAnalyzer::getQuads() const {
    return quads_;
}

void SemanticAnalyzer::backpatch(int quadId, const string& label) {
    if (quadId >= 0 && quadId < (int)quads_.size()) {
        quads_[quadId].result = label;
    }
}

vector<Quadruple> SemanticAnalyzer::extractLastQuads(int n) {
    vector<Quadruple> extracted;
    for (int i = 0; i < n && !quads_.empty(); i++) {
        extracted.push_back(quads_.back());
        quads_.pop_back();
    }
    std::reverse(extracted.begin(), extracted.end());
    return extracted;
}

void SemanticAnalyzer::restoreQuads(const vector<Quadruple>& quads) {
    for (const auto& q : quads) {
        quads_.push_back(q);
        quads_.back().id = (int)quads_.size() - 1;
    }
}

// --- 将变量名/临时变量/常量映射为 Jack VM 操作数 ---
static string toVM(const string& operand, const map<string, SymbolEntry>& symTable, int tempBase) {
    if (operand.empty() || operand == "_") return "";

    // 常量
    if (isdigit(operand[0]) || operand[0] == '-') {
        return "constant " + operand;
    }
    // 字符常数
    if (operand[0] == '\'') {
        return "constant " + to_string((int)operand[1]);
    }
    // 临时变量 t0, t1, ...
    if (operand[0] == 't' && operand.size() > 1 && isdigit(operand[1])) {
        int tid = stoi(operand.substr(1));
        if (tid < 8) {
            return "temp " + to_string(tid);
        }
        // 超过 8 个临时变量，使用 local
        return "local " + to_string(tempBase + tid);
    }
    // 标签
    if (operand[0] == 'L' && operand.size() > 1 && isdigit(operand[1])) {
        return operand;  // 标签保持原样
    }
    // 变量名 → local N
    auto it = symTable.find(operand);
    if (it != symTable.end()) {
        return "local " + to_string(it->second.offset);
    }
    // 未知变量，当作 local（隐式声明）
    return operand;
}

// --- 输出四元式 ---
void SemanticAnalyzer::dumpQuads(const string& filename) const {
    ofstream f(filename);
    f << "========== 四元式序列 ==========\n\n";
    f << std::left
      << std::setw(6) << "ID"
      << std::setw(12) << "OP"
      << std::setw(12) << "ARG1"
      << std::setw(12) << "ARG2"
      << std::setw(12) << "RESULT" << "\n";
    f << "----  ---------  ---------  ---------  ---------\n";
    for (const auto& q : quads_) {
        f << std::left
          << std::setw(6) << q.id
          << std::setw(12) << q.op
          << std::setw(12) << (q.arg1.empty() ? "-" : q.arg1)
          << std::setw(12) << (q.arg2.empty() ? "-" : q.arg2)
          << std::setw(12) << (q.result.empty() ? "-" : q.result) << "\n";
    }
    f.close();
}

// --- 输出 Jack VM 目标代码 ---
void SemanticAnalyzer::dumpTarget(const string& filename) const {
    ofstream f(filename);

    int numLocals = varCount_;  // 只计算用户声明的变量
    int tempBase = varCount_;  // 临时变量使用 temp 段，不占 local

    // 函数头
    f << "function Main.main " << numLocals << "\n";

    for (const auto& q : quads_) {
        string a1 = toVM(q.arg1, symbolTable_, tempBase);
        string a2 = toVM(q.arg2, symbolTable_, tempBase);
        string rs = toVM(q.result, symbolTable_, tempBase);

        // 标签
        if (q.op == "label") {
            f << "label " << q.arg1 << "\n";
        }
        // 无条件跳转
        else if (q.op == "goto") {
            f << "goto " << q.arg1 << "\n";
        }
        // 条件跳转: if_false cond label
        else if (q.op == "if_false") {
            f << "push " << a1 << "\n";
            f << "not\n";
            f << "if-goto " << q.result << "\n";
        }
        // 赋值: (=, src, _, dst)
        else if (q.op == "=") {
            f << "push " << a1 << "\n";
            f << "pop " << rs << "\n";
        }
        // 算术/关系运算: (op, a, b, result)
        else if (q.op == "+" || q.op == "-" || q.op == "*" || q.op == "/" ||
                 q.op == "<" || q.op == ">" || q.op == "==") {
            f << "push " << a1 << "\n";
            f << "push " << a2 << "\n";
            if (q.op == "+") f << "add\n";
            else if (q.op == "-") f << "sub\n";
            else if (q.op == "*") f << "call Math.multiply 2\n";
            else if (q.op == "/") f << "call Math.divide 2\n";
            else if (q.op == "<") f << "lt\n";
            else if (q.op == ">") f << "gt\n";
            else if (q.op == "==") f << "eq\n";
            f << "pop " << rs << "\n";
        }
        // 成员访问: (., base, member, result)
        else if (q.op == ".") {
            // 简化：将 base.member 视为 base + memberOffset
            f << "push " << a1 << "\n";
            f << "pop " << rs << "\n";
        }
        // 数组下标: ([], base, index, result)
        else if (q.op == "[]") {
            f << "push " << a1 << "\n";
            f << "push " << a2 << "\n";
            f << "add\n";
            f << "pop " << rs << "\n";
        }
        // 函数调用: (call, name, nArgs, result)
        else if (q.op == "call") {
            string funcName = q.arg1;
            // 将 read/write/println 映射为 Jack OS 函数
            if (funcName == "read") funcName = "Keyboard.readInt";
            else if (funcName == "write") funcName = "Output.printInt";
            else if (funcName == "println") funcName = "Output.println";
            f << "call " << funcName << " " << q.arg2 << "\n";
            // 有返回值：pop 到目标位置
            if (!q.result.empty() && q.result != "_") {
                f << "pop " << rs << "\n";
            } else {
                // 无返回值：丢弃
                f << "pop temp 0\n";
            }
        }
        // 参数传递: (param, arg, _, _)
        else if (q.op == "param") {
            f << "push " << a1 << "\n";
        }
        // 返回: (return, value, _, _)
        else if (q.op == "return") {
            if (!q.arg1.empty()) {
                f << "push " << a1 << "\n";
                f << "return\n";
            } else {
                f << "push constant 0\n";
                f << "return\n";
            }
        }
    }

    // 如果最后一条不是 return，添加默认 return
    if (quads_.empty() || quads_.back().op != "return") {
        f << "push constant 0\n";
        f << "return\n";
    }

    f.close();
}
