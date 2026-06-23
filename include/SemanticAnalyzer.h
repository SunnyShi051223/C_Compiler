#pragma once

#include "Token.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

// --- 四元式 ---
struct Quadruple {
    int id;
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

// --- 符号表条目 ---
struct SymbolEntry {
    std::string name;
    std::string type;     // "int" / "char" / "double" / struct名
    int offset;           // 局部变量偏移量
    bool isStruct;
};

// --- 函数签名 ---
struct FuncEntry {
    std::string name;
    std::vector<std::string> paramTypes;
    std::string returnType;
};

// --- 作用域 ---
struct Scope {
    std::map<std::string, SymbolEntry> symbols;
    int baseOffset;  // 该作用域的起始偏移量
};

// --- 结构体类型定义 ---
struct StructDef {
    std::string name;                          // 结构体名
    std::vector<std::string> memberNames;      // 成员名列表
    std::vector<std::string> memberTypes;      // 成员类型列表
};

// --- 语义分析器 ---
class SemanticAnalyzer {
public:
    SemanticAnalyzer();

    // 临时变量管理
    std::string newTemp();

    // 标签管理
    std::string newLabel();

    // 作用域管理
    void enterScope();
    void exitScope();
    int getCurrentScopeLevel() const;

    // 符号表操作
    void declareVariable(const std::string& name, const std::string& type);
    bool isDeclared(const std::string& name) const;
    std::string getType(const std::string& name) const;
    int getOffset(const std::string& name) const;
    int getVariableCount() const;

    // 函数表
    void declareFunction(const std::string& name, const std::vector<std::string>& paramTypes, const std::string& returnType);
    bool isFunctionDeclared(const std::string& name) const;
    const FuncEntry* getFunction(const std::string& name) const;
    void checkFunctionCall(const std::string& name, int argCount, int line);

    // 结构体类型表
    void defineStruct(const std::string& name);
    void addStructMember(const std::string& structName, const std::string& memberType, const std::string& memberName);
    bool isStructDefined(const std::string& name) const;
    bool isStructMember(const std::string& structName, const std::string& memberName) const;
    int getMemberOffset(const std::string& structName, const std::string& memberName) const;
    int getStructMemberCount(const std::string& structName) const;

    // 语义检查
    void checkAssignment(const std::string& lhs, const std::string& rhs, int line);
    void checkDeclared(const std::string& name, int line);
    void checkStructDefined(const std::string& name, int line);
    void checkIsStruct(const std::string& varName, int line);
    void checkMemberAccess(const std::string& varName, const std::string& member, int line);

    // 四元式生成
    int emit(const std::string& op, const std::string& arg1,
             const std::string& arg2, const std::string& result);
    const std::vector<Quadruple>& getQuads() const;

    // 回填（用于控制流）
    void backpatch(int quadId, const std::string& label);

    // 提取最后 N 条四元式（用于重排控制流）
    std::vector<Quadruple> extractLastQuads(int n);
    // 恢复四元式
    void restoreQuads(const std::vector<Quadruple>& quads);
    // 获取当前四元式数量
    int getQuadCount() const;
    // 提取从指定位置开始的所有四元式
    std::vector<Quadruple> extractQuadsFrom(int start);

    // 输出
    void dumpQuads(const std::string& filename) const;
    void dumpTarget(const std::string& filename) const;

private:
    int tempCount_;
    int labelCount_;
    int varCount_;
    std::vector<Quadruple> quads_;
    std::vector<Scope> scopeStack_;              // 作用域栈
    std::map<std::string, StructDef> structTable_;  // 结构体类型表
    std::map<std::string, FuncEntry> funcTable_;    // 函数表

    // 在作用域栈中查找变量（从内到外）
    SymbolEntry* lookup(const std::string& name);
    const SymbolEntry* lookup(const std::string& name) const;

public:
    // 结构体上下文（用于在声明语句中注册结构体成员）
    bool inStructBody_ = false;
    std::string currentStructName_;
};
