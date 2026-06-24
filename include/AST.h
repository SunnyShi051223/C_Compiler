#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>

// --- AST 节点类型枚举 ---
enum class ASTNodeType {
    // 程序结构
    Program,            // 程序: 子节点为语句列表

    // 声明
    VarDecl,            // 变量声明: value=类型, children=[IdLeaf]
    StructDecl,         // 结构体定义: value=结构体名, children=[成员声明...]
    StructVarDecl,      // 结构体变量声明: value=结构体类型, children=[IdLeaf]

    // 语句
    AssignStmt,         // 赋值语句: children=[LValue, Expr]
    IfStmt,             // if语句: children=[Cond, ThenBlock, ElseBlock] (ElseBlock可选)
    WhileStmt,          // while语句: children=[Cond, BodyBlock]
    ReturnStmt,         // return语句: children=[Expr]
    Block,              // 语句块: children=[Stmt, Stmt, ...]
    ExprStmt,           // 表达式语句: children=[Expr]

    // 左值 (LValue)
    LValueId,           // 简单变量: value=变量名
    LValueMember,       // 成员访问: value=成员名, children=[BaseExpr]
    LValueArray,        // 数组下标: children=[BaseExpr, IndexExpr]

    // 表达式
    BinaryExpr,         // 二元运算: value=运算符, children=[Left, Right]
    AssignExpr,         // 赋值表达式: children=[LValue, Expr]
    CallExpr,           // 函数调用: value=函数名, children=[Arg1, Arg2, ...]
    ParenExpr,          // 括号表达式: children=[InnerExpr]

    // 叶子节点
    IdLeaf,             // 标识符: value=变量名
    NumLeaf,            // 数字常量: value=数字字符串
    CharLeaf,           // 字符常量: value=字符字符串
};

// --- AST 节点 ---
struct ASTNode {
    ASTNodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;
    int line;

    ASTNode(ASTNodeType t, const std::string& v = "", int l = 0)
        : type(t), value(v), line(l) {}

    // 添加子节点
    void addChild(std::shared_ptr<ASTNode> child) {
        children.push_back(std::move(child));
    }
};

// --- AST 工具函数 ---
std::string astNodeTypeToString(ASTNodeType type);
void dumpAST(const std::shared_ptr<ASTNode>& root, const std::string& filename);
void printAST(std::ofstream& f, const std::shared_ptr<ASTNode>& node, int depth = 0);
