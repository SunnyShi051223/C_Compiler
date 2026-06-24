#pragma once

#include "AST.h"
#include "SemanticAnalyzer.h"
#include <string>
#include <memory>

// --- 代码生成访问器 ---
// 遍历 AST 生成四元式，替代原有的归约时内联代码生成
class CodegenVisitor {
public:
    CodegenVisitor(SemanticAnalyzer& sem);

    // 遍历 AST 生成四元式
    void visit(const std::shared_ptr<ASTNode>& node);

private:
    SemanticAnalyzer& sem_;

    // 语句访问（无返回值）
    void visitProgram(const std::shared_ptr<ASTNode>& node);
    void visitVarDecl(const std::shared_ptr<ASTNode>& node);
    void visitStructDecl(const std::shared_ptr<ASTNode>& node);
    void visitStructVarDecl(const std::shared_ptr<ASTNode>& node);
    void visitAssignStmt(const std::shared_ptr<ASTNode>& node);
    void visitIfStmt(const std::shared_ptr<ASTNode>& node);
    void visitWhileStmt(const std::shared_ptr<ASTNode>& node);
    void visitReturnStmt(const std::shared_ptr<ASTNode>& node);
    void visitBlock(const std::shared_ptr<ASTNode>& node);
    void visitExprStmt(const std::shared_ptr<ASTNode>& node);

    // 表达式访问（返回结果地址：变量名/临时变量/常量）
    std::string visitExpr(const std::shared_ptr<ASTNode>& node);
    std::string visitBinaryExpr(const std::shared_ptr<ASTNode>& node);
    std::string visitAssignExpr(const std::shared_ptr<ASTNode>& node);
    std::string visitCallExpr(const std::shared_ptr<ASTNode>& node);
    std::string visitParenExpr(const std::shared_ptr<ASTNode>& node);

    // 叶子节点访问
    std::string visitIdLeaf(const std::shared_ptr<ASTNode>& node);
    std::string visitNumLeaf(const std::shared_ptr<ASTNode>& node);
    std::string visitCharLeaf(const std::shared_ptr<ASTNode>& node);

    // LValue 访问（返回目标地址）
    std::string visitLValue(const std::shared_ptr<ASTNode>& node);
};
