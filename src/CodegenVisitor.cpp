#include "../include/CodegenVisitor.h"
#include <iostream>

using std::string;
using std::shared_ptr;
using std::vector;

CodegenVisitor::CodegenVisitor(SemanticAnalyzer& sem) : sem_(sem) {}

// ==================== 主入口 ====================

void CodegenVisitor::visit(const shared_ptr<ASTNode>& node) {
    if (!node) return;

    switch (node->type) {
        case ASTNodeType::Program:       visitProgram(node); break;
        case ASTNodeType::VarDecl:       visitVarDecl(node); break;
        case ASTNodeType::StructDecl:    visitStructDecl(node); break;
        case ASTNodeType::StructVarDecl: visitStructVarDecl(node); break;
        case ASTNodeType::AssignStmt:    visitAssignStmt(node); break;
        case ASTNodeType::IfStmt:        visitIfStmt(node); break;
        case ASTNodeType::WhileStmt:     visitWhileStmt(node); break;
        case ASTNodeType::ReturnStmt:    visitReturnStmt(node); break;
        case ASTNodeType::Block:         visitBlock(node); break;
        case ASTNodeType::ExprStmt:      visitExprStmt(node); break;
        // 表达式节点在 visitExpr 中处理
        default:
            // 其他节点类型由 visitExpr 处理
            visitExpr(node);
            break;
    }
}

// ==================== 语句 ====================

void CodegenVisitor::visitProgram(const shared_ptr<ASTNode>& node) {
    for (const auto& child : node->children) {
        visit(child);
    }
}

void CodegenVisitor::visitVarDecl(const shared_ptr<ASTNode>& node) {
    // node->value = 类型 (int/char/double)
    // node->children[0] = IdLeaf (变量名)
    if (!node->children.empty()) {
        string varName = node->children[0]->value;
        sem_.declareVariable(varName, node->value);
    }
}

void CodegenVisitor::visitStructDecl(const shared_ptr<ASTNode>& node) {
    // node->value = 结构体名
    // node->children = 成员声明列表
    sem_.defineStruct(node->value);
    sem_.inStructBody_ = true;
    sem_.currentStructName_ = node->value;

    for (const auto& child : node->children) {
        if (child->type == ASTNodeType::VarDecl) {
            sem_.addStructMember(node->value, child->value, child->children[0]->value);
        }
    }

    sem_.inStructBody_ = false;
    sem_.currentStructName_ = "";
}

void CodegenVisitor::visitStructVarDecl(const shared_ptr<ASTNode>& node) {
    // node->value = 结构体类型名
    // node->children[0] = IdLeaf (变量名)
    if (!node->children.empty()) {
        sem_.checkStructDefined(node->value, node->line);
        sem_.declareVariable(node->children[0]->value, node->value);
    }
}

void CodegenVisitor::visitAssignStmt(const shared_ptr<ASTNode>& node) {
    // children[0] = LValue, children[1] = Expr
    if (node->children.size() < 2) return;

    string rhsAddr = visitExpr(node->children[1]);
    string lhsAddr = visitLValue(node->children[0]);

    // 语义检查
    if (node->children[0]->type == ASTNodeType::LValueId) {
        sem_.checkDeclared(lhsAddr, node->line);
        sem_.checkAssignment(lhsAddr, rhsAddr, node->line);
    }

    sem_.emit("=", rhsAddr, "", lhsAddr);
}

void CodegenVisitor::visitIfStmt(const shared_ptr<ASTNode>& node) {
    // children[0] = Cond, children[1] = ThenBlock, children[2] = ElseBlock (可选)
    if (node->children.size() < 2) return;

    string condAddr = visitExpr(node->children[0]);
    string L1 = sem_.newLabel();
    string L2 = sem_.newLabel();

    // if_false cond → L1 (else分支)
    int j = sem_.emit("if_false", condAddr, "", L1);

    // then 分支
    visit(node->children[1]);
    sem_.emit("goto", L2, "", "");

    // else 分支
    sem_.backpatch(j, L1);
    sem_.emit("label", L1, "", "");
    if (node->children.size() > 2) {
        visit(node->children[2]);
    }
    sem_.backpatch(j, L1);  // 确保回填
    sem_.emit("label", L2, "", "");
}

void CodegenVisitor::visitWhileStmt(const shared_ptr<ASTNode>& node) {
    // children[0] = Cond, children[1] = BodyBlock
    if (node->children.size() < 2) return;

    string L0 = sem_.newLabel();
    string L1 = sem_.newLabel();

    // 循环头标签
    sem_.emit("label", L0, "", "");

    // 条件计算
    string condAddr = visitExpr(node->children[0]);
    int j = sem_.emit("if_false", condAddr, "", L1);

    // 循环体
    visit(node->children[1]);
    sem_.emit("goto", L0, "", "");

    // 循环出口
    sem_.backpatch(j, L1);
    sem_.emit("label", L1, "", "");
}

void CodegenVisitor::visitReturnStmt(const shared_ptr<ASTNode>& node) {
    if (!node->children.empty()) {
        string addr = visitExpr(node->children[0]);
        sem_.emit("return", addr, "", "");
    } else {
        sem_.emit("return", "", "", "");
    }
}

void CodegenVisitor::visitBlock(const shared_ptr<ASTNode>& node) {
    // 进入新作用域
    sem_.enterScope();

    for (const auto& child : node->children) {
        visit(child);
    }

    // 退出作用域
    sem_.exitScope();
}

void CodegenVisitor::visitExprStmt(const shared_ptr<ASTNode>& node) {
    if (!node->children.empty()) {
        visitExpr(node->children[0]);
    }
}

// ==================== 表达式 ====================

string CodegenVisitor::visitExpr(const shared_ptr<ASTNode>& node) {
    if (!node) return "";

    switch (node->type) {
        case ASTNodeType::BinaryExpr:  return visitBinaryExpr(node);
        case ASTNodeType::AssignExpr:  return visitAssignExpr(node);
        case ASTNodeType::CallExpr:    return visitCallExpr(node);
        case ASTNodeType::ParenExpr:   return visitParenExpr(node);
        case ASTNodeType::IdLeaf:      return visitIdLeaf(node);
        case ASTNodeType::NumLeaf:     return visitNumLeaf(node);
        case ASTNodeType::CharLeaf:    return visitCharLeaf(node);
        case ASTNodeType::LValueId:    return visitLValue(node);
        case ASTNodeType::LValueMember:return visitLValue(node);
        case ASTNodeType::LValueArray: return visitLValue(node);
        default:
            // 对于其他节点类型，尝试访问子节点
            if (!node->children.empty()) {
                return visitExpr(node->children[0]);
            }
            return "";
    }
}

string CodegenVisitor::visitBinaryExpr(const shared_ptr<ASTNode>& node) {
    // value = 运算符, children = [Left, Right]
    if (node->children.size() < 2) return "";

    string leftAddr = visitExpr(node->children[0]);
    string rightAddr = visitExpr(node->children[1]);
    string t = sem_.newTemp();
    sem_.emit(node->value, leftAddr, rightAddr, t);
    return t;
}

string CodegenVisitor::visitAssignExpr(const shared_ptr<ASTNode>& node) {
    // children[0] = LValue, children[1] = Expr (赋值表达式，如 a = b = c)
    if (node->children.size() < 2) return "";

    string rhsAddr = visitExpr(node->children[1]);
    string lhsAddr = visitLValue(node->children[0]);

    sem_.emit("=", rhsAddr, "", lhsAddr);
    return lhsAddr;
}

string CodegenVisitor::visitCallExpr(const shared_ptr<ASTNode>& node) {
    // value = 函数名, children = [Arg1, Arg2, ...]
    string funcName = node->value;
    int argCount = (int)node->children.size();

    // 参数检查
    sem_.checkFunctionCall(funcName, argCount, node->line);

    // 特殊处理 read (无参数，返回值)
    if (funcName == "read") {
        string t = sem_.newTemp();
        sem_.emit("call", "read", "0", t);
        return t;
    }

    // 特殊处理 write (一个参数，无返回值)
    if (funcName == "write") {
        if (!node->children.empty()) {
            string argAddr = visitExpr(node->children[0]);
            sem_.emit("param", argAddr, "", "");
        }
        sem_.emit("call", "write", "1", "_");
        return "_";
    }

    // 特殊处理 println (无参数，无返回值)
    if (funcName == "println") {
        sem_.emit("call", "println", "0", "_");
        return "_";
    }

    // 一般函数调用
    for (const auto& arg : node->children) {
        string argAddr = visitExpr(arg);
        sem_.emit("param", argAddr, "", "");
    }

    string t = sem_.newTemp();
    sem_.emit("call", funcName, std::to_string(argCount), t);
    return t;
}

string CodegenVisitor::visitParenExpr(const shared_ptr<ASTNode>& node) {
    // children[0] = 内部表达式
    if (node->children.empty()) return "";
    return visitExpr(node->children[0]);
}

// ==================== 叶子节点 ====================

string CodegenVisitor::visitIdLeaf(const shared_ptr<ASTNode>& node) {
    // 检查变量是否已声明
    if (!sem_.isDeclared(node->value)) {
        sem_.checkDeclared(node->value, node->line);
        // 隐式声明为 int（兼容旧行为）
        sem_.declareVariable(node->value, "int");
    }
    return node->value;
}

string CodegenVisitor::visitNumLeaf(const shared_ptr<ASTNode>& node) {
    return node->value;
}

string CodegenVisitor::visitCharLeaf(const shared_ptr<ASTNode>& node) {
    return node->value;
}

// ==================== LValue ====================

string CodegenVisitor::visitLValue(const shared_ptr<ASTNode>& node) {
    if (!node) return "";

    switch (node->type) {
        case ASTNodeType::LValueId:
            // 简单变量，返回变量名
            if (!sem_.isDeclared(node->value)) {
                sem_.checkDeclared(node->value, node->line);
                sem_.declareVariable(node->value, "int");
            }
            return node->value;

        case ASTNodeType::LValueMember: {
            // 成员访问: value=成员名, children=[BaseExpr]
            if (node->children.empty()) return "";
            string baseAddr = visitExpr(node->children[0]);
            string memberName = node->value;

            // 语义检查
            if (node->children[0]->type == ASTNodeType::IdLeaf) {
                sem_.checkMemberAccess(node->children[0]->value, memberName, node->line);
            }

            string t = sem_.newTemp();
            sem_.emit(".", baseAddr, memberName, t);
            return t;
        }

        case ASTNodeType::LValueArray: {
            // 数组下标: children=[BaseExpr, IndexExpr]
            if (node->children.size() < 2) return "";
            string baseAddr = visitExpr(node->children[0]);
            string indexAddr = visitExpr(node->children[1]);
            string t = sem_.newTemp();
            sem_.emit("[]", baseAddr, indexAddr, t);
            return t;
        }

        default:
            // 如果不是 LValue 节点，当作表达式处理
            return visitExpr(node);
    }
}
