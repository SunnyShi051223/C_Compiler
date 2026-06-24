#include "../include/AST.h"
#include <iostream>

using std::string;
using std::vector;
using std::ofstream;
using std::shared_ptr;

string astNodeTypeToString(ASTNodeType type) {
    switch (type) {
        case ASTNodeType::Program:        return "Program";
        case ASTNodeType::VarDecl:        return "VarDecl";
        case ASTNodeType::StructDecl:     return "StructDecl";
        case ASTNodeType::StructVarDecl:  return "StructVarDecl";
        case ASTNodeType::AssignStmt:     return "AssignStmt";
        case ASTNodeType::IfStmt:         return "IfStmt";
        case ASTNodeType::WhileStmt:      return "WhileStmt";
        case ASTNodeType::ReturnStmt:     return "ReturnStmt";
        case ASTNodeType::Block:          return "Block";
        case ASTNodeType::ExprStmt:       return "ExprStmt";
        case ASTNodeType::LValueId:       return "LValueId";
        case ASTNodeType::LValueMember:   return "LValueMember";
        case ASTNodeType::LValueArray:    return "LValueArray";
        case ASTNodeType::BinaryExpr:     return "BinaryExpr";
        case ASTNodeType::AssignExpr:     return "AssignExpr";
        case ASTNodeType::CallExpr:       return "CallExpr";
        case ASTNodeType::ParenExpr:      return "ParenExpr";
        case ASTNodeType::IdLeaf:         return "IdLeaf";
        case ASTNodeType::NumLeaf:        return "NumLeaf";
        case ASTNodeType::CharLeaf:       return "CharLeaf";
        default:                          return "Unknown";
    }
}

void printAST(ofstream& f, const shared_ptr<ASTNode>& node, int depth) {
    if (!node) return;

    // 缩进
    for (int i = 0; i < depth; i++) f << "│  ";

    // 节点信息
    f << "├─ " << astNodeTypeToString(node->type);
    if (!node->value.empty()) {
        f << "  \"" << node->value << "\"";
    }
    if (node->line > 0) {
        f << "  (line " << node->line << ")";
    }
    f << "\n";

    // 递归打印子节点
    for (const auto& child : node->children) {
        printAST(f, child, depth + 1);
    }
}

void dumpAST(const shared_ptr<ASTNode>& root, const string& filename) {
    ofstream f(filename);
    f << "========== 抽象语法树 (AST) ==========\n\n";
    if (root) {
        printAST(f, root, 0);
    } else {
        f << "(空 AST)\n";
    }
    f.close();
}
