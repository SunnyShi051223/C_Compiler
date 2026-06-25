#include "../include/PrecedenceParser.h"
#include <iostream>
#include <algorithm>
#include <functional>

using std::string;
using std::vector;
using std::ofstream;
using std::cerr;
using std::endl;
using std::to_string;

PrecedenceParser::PrecedenceParser(const Grammar& grammar, ErrorHandler& errorHandler,
                                   SemanticAnalyzer& semAnalyzer)
    : grammar_(grammar), errorHandler_(errorHandler), sem_(semAnalyzer), nextNodeId_(0), astRoot_(nullptr) {}

string PrecedenceParser::tokenToTerminal(const Token& token) const {
    switch (token.type) {
        case TokenType::KEYWORD:
            if (token.value == "main" || token.value == "read" || token.value == "write")
                return "id";
            return token.value;
        case TokenType::IDENTIFIER:  return "id";
        case TokenType::NUMBER:      return "num";
        case TokenType::CHAR_LITERAL:return "char";
        case TokenType::OPERATOR:    return token.value;
        case TokenType::DELIMITER:   return token.value;
        case TokenType::END_OF_FILE: return "$";
        default:                     return token.value;
    }
}

void PrecedenceParser::push(const StackSymbol& sym) { stack_.push_back(sym); }

int PrecedenceParser::findTopTerminal() const {
    for (int i = (int)stack_.size() - 1; i >= 0; i--) {
        if (stack_[i].isTerminal) return i;
    }
    return -1;
}

string PrecedenceParser::stackToString() const {
    string s;
    for (size_t i = 0; i < stack_.size(); i++) {
        if (i > 0) s += " ";
        s += stack_[i].symbol;
    }
    return s;
}

int PrecedenceParser::createNode(const string& symbol, bool isTerminal, int line) {
    TreeNode node;
    node.id = nextNodeId_++;
    node.symbol = symbol;
    node.isTerminal = isTerminal;
    node.line = line;
    treeNodes_.push_back(node);
    return node.id;
}

// ==================== 语义动作 ====================
void PrecedenceParser::semanticAction(const Production& prod, int start) {
    const auto& rhs = prod.right;
    size_t len = rhs.size();

    // 声明
    if (len == 2 && (rhs[0] == "int" || rhs[0] == "char" || rhs[0] == "double")) {
        sem_.declareVariable(stack_[start + 1].addr, rhs[0]);
    }
    // 数组声明：int/char/double id [ num ]
    else if (len == 5 && (rhs[0] == "int" || rhs[0] == "char" || rhs[0] == "double") &&
             rhs[2] == "[" && rhs[4] == "]") {
        string varName = stack_[start + 1].addr;
        string arraySize = stack_[start + 3].addr;
        sem_.declareVariable(varName, rhs[0] + "[]");  // 标记为数组类型
        sem_.emit("alloc", arraySize, "", varName);    // 分配堆内存
    }
    else if (len == 3 && rhs[0] == "struct" && rhs[2] == "id") {
        string structType = stack_[start + 1].addr;
        string varName = stack_[start + 2].addr;
        sem_.checkStructDefined(structType, stack_[start].line);
        sem_.declareVariable(varName, structType);
        // 为结构体分配堆内存：call Memory.alloc size → pop 到变量
        int memberCount = sem_.getStructMemberCount(structType);
        if (memberCount > 0) {
            sem_.emit("alloc", std::to_string(memberCount), "", varName);
        }
    }
    else if (len == 5 && rhs[0] == "struct" && rhs[3] == "P") {
        string structName = stack_[start + 1].addr;
        sem_.defineStruct(structName);
        // 从 P 节点提取成员声明并注册到结构体
        if (stack_[start + 3].astNode) {
            for (const auto& child : stack_[start + 3].astNode->children) {
                if (child->type == ASTNodeType::VarDecl && !child->children.empty()) {
                    string memberType = child->value;
                    string memberName = child->children[0]->value;
                    sem_.addStructMember(structName, memberType, memberName);
                }
            }
        }
    }
    // 赋值
    else if (len == 3 && rhs[0] == "id" && rhs[1] == "=") {
        sem_.checkDeclared(stack_[start].addr, stack_[start].line);
        sem_.checkAssignment(stack_[start].addr, stack_[start + 2].addr, stack_[start].line);
        sem_.emit("=", stack_[start + 2].addr, "", stack_[start].addr);
    }
    // 控制流
    else if (len == 7 && rhs[0] == "while") {
        string cond = stack_[start + 2].addr;
        string L0 = sem_.newLabel(), L1 = sem_.newLabel();

        // 提取循环体四元式（在条件之后生成的）
        int condEnd = stack_[start + 2].quadCount;
        auto bodyQuads = sem_.extractQuadsFrom(condEnd);
        // 提取条件结果四元式
        auto condQuads = sem_.extractLastQuads(1);

        // 正确顺序: label L0 | 条件 | if_false L1 | 循环体 | goto L0 | label L1
        sem_.emit("label", L0, "", "");
        sem_.restoreQuads(condQuads);
        int j = sem_.emit("if_false", cond, "", "");
        sem_.restoreQuads(bodyQuads);
        sem_.emit("goto", L0, "", "");
        sem_.backpatch(j, L1);
        sem_.emit("label", L1, "", "");
    }
    else if (len == 11 && rhs[0] == "if") {
        string cond = stack_[start + 2].addr;
        string L1 = sem_.newLabel(), L2 = sem_.newLabel();

        int condEnd = stack_[start + 2].quadCount;
        auto allBodyQuads = sem_.extractQuadsFrom(condEnd);
        auto condQuads = sem_.extractLastQuads(1);

        // 分割 then 和 else 的四元式（各占一半）
        int mid = (int)allBodyQuads.size() / 2;
        vector<Quadruple> thenQuads(allBodyQuads.begin(), allBodyQuads.begin() + mid);
        vector<Quadruple> elseQuads(allBodyQuads.begin() + mid, allBodyQuads.end());

        // 正确顺序: 条件 | if_false L1 | then体 | goto L2 | label L1 | else体 | label L2
        sem_.restoreQuads(condQuads);
        int j = sem_.emit("if_false", cond, "", "");
        sem_.restoreQuads(thenQuads);
        int g = sem_.emit("goto", L2, "", "");
        sem_.backpatch(j, L1);
        sem_.emit("label", L1, "", "");
        sem_.restoreQuads(elseQuads);
        sem_.backpatch(g, L2);
        sem_.emit("label", L2, "", "");
    }
    // if ( E ) { P } （无 else）
    else if (len == 7 && rhs[0] == "if") {
        string cond = stack_[start + 2].addr;
        string L1 = sem_.newLabel();

        int condEnd = stack_[start + 2].quadCount;
        auto bodyQuads = sem_.extractQuadsFrom(condEnd);
        auto condQuads = sem_.extractLastQuads(1);

        // 正确顺序: 条件 | if_false L1 | then体 | label L1
        sem_.restoreQuads(condQuads);
        int j = sem_.emit("if_false", cond, "", "");
        sem_.restoreQuads(bodyQuads);
        sem_.backpatch(j, L1);
        sem_.emit("label", L1, "", "");
    }
    else if (len == 2 && rhs[0] == "return") {
        sem_.emit("return", stack_[start + 1].addr, "", "");
    }
    // E op E
    else if (len == 3 && rhs[0] == "E" && rhs[2] == "E") {
        string t = sem_.newTemp();
        sem_.emit(stack_[start + 1].symbol, stack_[start].addr, stack_[start + 2].addr, t);
        stack_[start].addr = t;
        stack_[start].quadCount = sem_.getQuadCount();
    }
    // 函数调用表达式: id(E,E)
    else if (len == 6 && rhs[0] == "id" && rhs[2] == "E" && rhs[4] == "E") {
        string fn = stack_[start].addr;
        string arg1 = stack_[start + 2].addr;  // 第1个参数（结构体变量）
        string arg2 = stack_[start + 4].addr;  // 第2个参数（成员名或索引）

        // get(p, member) → 自动将成员名替换为偏移量
        if (fn == "get") {
            string structType = sem_.getType(arg1);
            if (!structType.empty() && sem_.isStructMember(structType, arg2)) {
                int offset = sem_.getMemberOffset(structType, arg2);
                arg2 = std::to_string(offset);  // 替换为偏移量常量
            }
        }

        string t = sem_.newTemp();
        sem_.emit("param", arg1, "", "");
        sem_.emit("param", arg2, "", "");
        sem_.emit("call", fn, "2", t);
        stack_[start].addr = t;
        stack_[start].quadCount = sem_.getQuadCount();
    }
    // 函数调用表达式: id(E,E,E) — 三参数（8个符号：id ( E , E , E )）
    else if (len == 8 && rhs[0] == "id" && rhs[2] == "E" && rhs[4] == "E" && rhs[6] == "E") {
        string fn = stack_[start].addr;
        string arg1 = stack_[start + 2].addr;  // 第1个参数（结构体变量）
        string arg2 = stack_[start + 4].addr;  // 第2个参数（成员名或偏移量）
        string arg3 = stack_[start + 6].addr;  // 第3个参数（值）

        // set(p, member, value) → 自动将成员名替换为偏移量
        if (fn == "set") {
            string structType = sem_.getType(arg1);
            if (!structType.empty() && sem_.isStructMember(structType, arg2)) {
                int offset = sem_.getMemberOffset(structType, arg2);
                arg2 = std::to_string(offset);  // 替换为偏移量常量
            }
        }

        string t = sem_.newTemp();
        sem_.emit("param", arg1, "", "");
        sem_.emit("param", arg2, "", "");
        sem_.emit("param", arg3, "", "");
        sem_.emit("call", fn, "3", t);
        stack_[start].addr = t;
        stack_[start].quadCount = sem_.getQuadCount();
    }
    // 函数调用表达式: id(E)
    else if (len == 4 && rhs[0] == "id" && rhs[2] == "E") {
        string fn = stack_[start].addr;
        string arg = stack_[start + 2].addr;
        if (fn == "write") {
            sem_.emit("param", arg, "", "");
            sem_.emit("call", "write", "1", "_");
        } else if (fn == "read") {
            sem_.emit("call", "read", "0", arg);
        } else {
            string t = sem_.newTemp();
            sem_.emit("param", arg, "", "");
            sem_.emit("call", fn, "1", t);
            stack_[start].addr = t;
        }
        stack_[start].quadCount = sem_.getQuadCount();
    }
    // 函数调用表达式: id()
    else if (len == 3 && rhs[0] == "id" && rhs[2] == ")") {
        string t = sem_.newTemp();
        sem_.emit("call", stack_[start].addr, "0", t);
        stack_[start].addr = t;
        stack_[start].quadCount = sem_.getQuadCount();
    }
    // ( E )
    else if (len == 3 && rhs[0] == "(" && rhs[2] == ")") {
        stack_[start].addr = stack_[start + 1].addr;
        stack_[start].quadCount = stack_[start + 1].quadCount;
    }
    // id
    else if (len == 1 && rhs[0] == "id") {
        if (!sem_.isDeclared(stack_[start].addr)) {
            sem_.checkDeclared(stack_[start].addr, stack_[start].line);
            sem_.declareVariable(stack_[start].addr, "int");
        }
        stack_[start].quadCount = sem_.getQuadCount();
    }
    // num / char
    else if (len == 1 && (rhs[0] == "num" || rhs[0] == "char")) {
        stack_[start].quadCount = sem_.getQuadCount();
    }
}

// ==================== AST 构建 ====================
using std::shared_ptr;
using std::make_shared;

shared_ptr<ASTNode> PrecedenceParser::tokenToASTLeaf(const Token& token) const {
    switch (token.type) {
        case TokenType::KEYWORD:
            if (token.value == "main" || token.value == "read" || token.value == "write")
                return make_shared<ASTNode>(ASTNodeType::IdLeaf, token.value, token.line);
            return nullptr;
        case TokenType::IDENTIFIER:
            return make_shared<ASTNode>(ASTNodeType::IdLeaf, token.value, token.line);
        case TokenType::NUMBER:
            return make_shared<ASTNode>(ASTNodeType::NumLeaf, token.value, token.line);
        case TokenType::CHAR_LITERAL:
            return make_shared<ASTNode>(ASTNodeType::CharLeaf, token.value, token.line);
        default:
            return nullptr;
    }
}

void PrecedenceParser::buildASTForProduction(const Production& prod, int start) {
    const auto& rhs = prod.right;
    size_t len = rhs.size();

    // P → S ; P | S ;
    if (prod.left == "P") {
        auto node = make_shared<ASTNode>(ASTNodeType::Program, "", stack_[start].line);
        for (size_t i = 0; i < len; i++) {
            if (stack_[start + i].astNode) {
                if (stack_[start + i].astNode->type == ASTNodeType::Program) {
                    for (const auto& c : stack_[start + i].astNode->children) node->addChild(c);
                } else if (!stack_[start + i].isTerminal) {
                    node->addChild(stack_[start + i].astNode);
                }
            }
        }
        stack_[start].astNode = node;
        return;
    }
    // S → int/char/double id
    if (len == 2 && (rhs[0] == "int" || rhs[0] == "char" || rhs[0] == "double")) {
        auto node = make_shared<ASTNode>(ASTNodeType::VarDecl, rhs[0], stack_[start].line);
        node->addChild(make_shared<ASTNode>(ASTNodeType::IdLeaf, stack_[start+1].addr, stack_[start+1].line));
        stack_[start].astNode = node;
        return;
    }
    // S → int/char/double id [ num ]  （数组声明）
    if (len == 5 && (rhs[0] == "int" || rhs[0] == "char" || rhs[0] == "double") &&
        rhs[2] == "[" && rhs[4] == "]") {
        auto node = make_shared<ASTNode>(ASTNodeType::VarDecl, rhs[0] + "[]", stack_[start].line);
        node->addChild(make_shared<ASTNode>(ASTNodeType::IdLeaf, stack_[start+1].addr, stack_[start+1].line));
        node->addChild(make_shared<ASTNode>(ASTNodeType::NumLeaf, stack_[start+3].addr, stack_[start+3].line));
        stack_[start].astNode = node;
        return;
    }
    // S → struct id { P }
    if (len == 5 && rhs[0] == "struct" && rhs[3] == "P") {
        auto node = make_shared<ASTNode>(ASTNodeType::StructDecl, stack_[start+1].addr, stack_[start].line);
        if (stack_[start+3].astNode) for (auto& c : stack_[start+3].astNode->children) node->addChild(c);
        stack_[start].astNode = node;
        return;
    }
    // S → struct id id
    if (len == 3 && rhs[0] == "struct" && rhs[2] == "id") {
        auto node = make_shared<ASTNode>(ASTNodeType::StructVarDecl, stack_[start+1].addr, stack_[start].line);
        node->addChild(make_shared<ASTNode>(ASTNodeType::IdLeaf, stack_[start+2].addr, stack_[start+2].line));
        stack_[start].astNode = node;
        return;
    }
    // S → id = E
    if (len == 3 && rhs[0] == "id" && rhs[1] == "=") {
        auto node = make_shared<ASTNode>(ASTNodeType::AssignStmt, "", stack_[start].line);
        node->addChild(make_shared<ASTNode>(ASTNodeType::LValueId, stack_[start].addr, stack_[start].line));
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        stack_[start].astNode = node;
        return;
    }
    // S → E
    if (len == 1 && rhs[0] == "E") {
        auto node = make_shared<ASTNode>(ASTNodeType::ExprStmt, "", stack_[start].line);
        if (stack_[start].astNode) node->addChild(stack_[start].astNode);
        stack_[start].astNode = node;
        return;
    }
    // S → while(E){P}
    if (len == 7 && rhs[0] == "while") {
        auto node = make_shared<ASTNode>(ASTNodeType::WhileStmt, "", stack_[start].line);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        if (stack_[start+5].astNode) node->addChild(stack_[start+5].astNode);
        stack_[start].astNode = node;
        return;
    }
    // S → if(E){P}else{P}
    if (len == 11 && rhs[0] == "if") {
        auto node = make_shared<ASTNode>(ASTNodeType::IfStmt, "", stack_[start].line);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        if (stack_[start+5].astNode) node->addChild(stack_[start+5].astNode);
        if (stack_[start+9].astNode) node->addChild(stack_[start+9].astNode);
        stack_[start].astNode = node;
        return;
    }
    // S → if(E){P}（无 else）
    if (len == 7 && rhs[0] == "if") {
        auto node = make_shared<ASTNode>(ASTNodeType::IfStmt, "", stack_[start].line);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        if (stack_[start+5].astNode) node->addChild(stack_[start+5].astNode);
        stack_[start].astNode = node;
        return;
    }
    // S → return E
    if (len == 2 && rhs[0] == "return") {
        auto node = make_shared<ASTNode>(ASTNodeType::ReturnStmt, "", stack_[start].line);
        if (stack_[start+1].astNode) node->addChild(stack_[start+1].astNode);
        stack_[start].astNode = node;
        return;
    }
    // E → E op E
    if (len == 3 && rhs[0] == "E" && rhs[2] == "E") {
        auto node = make_shared<ASTNode>(ASTNodeType::BinaryExpr, rhs[1], stack_[start].line);
        if (stack_[start].astNode) node->addChild(stack_[start].astNode);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        stack_[start].astNode = node;
        return;
    }
    // E → id()
    if (len == 3 && rhs[0] == "id" && rhs[2] == ")") {
        stack_[start].astNode = make_shared<ASTNode>(ASTNodeType::CallExpr, stack_[start].addr, stack_[start].line);
        return;
    }
    // E → id(E)
    if (len == 4 && rhs[0] == "id" && rhs[2] == "E") {
        auto node = make_shared<ASTNode>(ASTNodeType::CallExpr, stack_[start].addr, stack_[start].line);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        stack_[start].astNode = node;
        return;
    }
    // E → id(E,E)
    if (len == 6 && rhs[0] == "id" && rhs[2] == "E" && rhs[4] == "E") {
        auto node = make_shared<ASTNode>(ASTNodeType::CallExpr, stack_[start].addr, stack_[start].line);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        if (stack_[start+4].astNode) node->addChild(stack_[start+4].astNode);
        stack_[start].astNode = node;
        return;
    }
    // E → id(E,E,E) — 8个符号
    if (len == 8 && rhs[0] == "id" && rhs[2] == "E" && rhs[4] == "E" && rhs[6] == "E") {
        auto node = make_shared<ASTNode>(ASTNodeType::CallExpr, stack_[start].addr, stack_[start].line);
        if (stack_[start+2].astNode) node->addChild(stack_[start+2].astNode);
        if (stack_[start+4].astNode) node->addChild(stack_[start+4].astNode);
        if (stack_[start+6].astNode) node->addChild(stack_[start+6].astNode);
        stack_[start].astNode = node;
        return;
    }
    // E → (E)
    if (len == 3 && rhs[0] == "(" && rhs[2] == ")") {
        auto node = make_shared<ASTNode>(ASTNodeType::ParenExpr, "", stack_[start].line);
        if (stack_[start+1].astNode) node->addChild(stack_[start+1].astNode);
        stack_[start].astNode = node;
        return;
    }
    // E → id | num | char
    if (len == 1 && (rhs[0] == "id" || rhs[0] == "num" || rhs[0] == "char")) {
        if (!stack_[start].astNode) {
            ASTNodeType t = (rhs[0]=="id") ? ASTNodeType::IdLeaf :
                           (rhs[0]=="num") ? ASTNodeType::NumLeaf : ASTNodeType::CharLeaf;
            stack_[start].astNode = make_shared<ASTNode>(t, stack_[start].addr, stack_[start].line);
        }
        return;
    }
}

// ==================== 归约（标准算法：只从栈顶归约）====================
bool PrecedenceParser::reduce() {
    for (int len = (int)stack_.size(); len >= 1; len--) {
        int start = (int)stack_.size() - len;
        vector<string> candidate;
        for (int i = start; i < (int)stack_.size(); i++)
            candidate.push_back(stack_[i].symbol);

        for (const auto& prod : grammar_.getProductions()) {
            if (prod.right.size() == candidate.size()) {
                bool match = true;
                for (size_t i = 0; i < candidate.size(); i++) {
                    if (prod.right[i] != candidate[i]) { match = false; break; }
                }
                if (match) {
                    // 上下文敏感：S → E 只在语句边界归约
                    // 如果 E 前面是 ( { while if return = 或运算符，说明在表达式内部
                    if (prod.left == "S" && len == 1 && prod.right[0] == "E" && start > 0) {
                        auto& prev = stack_[start - 1];
                        if (prev.isTerminal) {
                            string p = prev.symbol;
                            if (p == "(" ||
                                p == "+" || p == "-" || p == "*" || p == "/" ||
                                p == "<" || p == ">" || p == "==" || p == "=" ||
                                p == ",") {
                                continue;  // 跳过，在表达式内部
                            }
                        }
                    }

                    int line = 0;
                    for (int i = start; i < (int)stack_.size(); i++)
                        if (stack_[i].isTerminal && stack_[i].line > 0) line = stack_[i].line;

                    semanticAction(prod, start);
                    buildASTForProduction(prod, start);

                    vector<int> childIds;
                    for (int i = start; i < (int)stack_.size(); i++)
                        childIds.push_back(stack_[i].treeNodeId);

                    string addr = stack_[start].addr;
                    auto astNd = stack_[start].astNode;
                    int nodeId = createNode(prod.left, false, line);
                    treeNodes_[nodeId].children = childIds;

                    for (int i = 0; i < len; i++) stack_.pop_back();
                    push({prod.left, false, line, nodeId, addr, "", sem_.getQuadCount(), astNd});

                    string action = "REDUCE: ";
                    for (size_t i = 0; i < candidate.size(); i++) {
                        if (i > 0) action += " ";
                        action += candidate[i];
                    }
                    action += " → " + prod.left + " (prod " + to_string(prod.id) + ")";
                    trace_.push_back({stackToString(), "", action});
                    return true;
                }
            }
        }
    }

    // 备用：归约栈中的单符号非终结符
    // 仅当下一个输入是 ; 或 $ 时触发（语句边界）
    // 避免在 while(E) 等内部误归约
    return false;
}

// ==================== 主解析函数 ====================
bool PrecedenceParser::parse(const vector<Token>& tokens) {
    stack_.clear();
    treeNodes_.clear();
    trace_.clear();
    nextNodeId_ = 0;
    astRoot_ = nullptr;

    push({"$", true, 0, createNode("$", true, 0), "", "", 0});

    size_t inputPos = 0;
    bool hasSyntaxError = false;

    while (inputPos < tokens.size()) {
        Token currentToken = tokens[inputPos];
        string currentTerminal = tokenToTerminal(currentToken);

        if (currentToken.type == TokenType::UNKNOWN) { inputPos++; continue; }

        if (currentTerminal == "$" && stack_.size() == 2 &&
            stack_[0].symbol == "$" && stack_[1].symbol == grammar_.getStartSymbol()) {
            astRoot_ = stack_[1].astNode;
            return !hasSyntaxError;
        }

        int topTermPos = findTopTerminal();
        if (topTermPos < 0) {
            if (!reduce()) {
                hasSyntaxError = true;
                errorHandler_.reportError(currentToken.line, currentToken.column,
                    "Syntax error near '" + currentToken.value + "'");
                inputPos++;
            }
            continue;
        }

        string topTerminal = stack_[topTermPos].symbol;
        string relation = grammar_.getRelation(topTerminal, currentTerminal);

        if (relation == "<" || relation == "=") {
            // 特殊处理：} 到达时，若栈顶是非终结符（如 { S } 中的 S），先归约
            if (currentTerminal == "}" && !stack_.empty() && !stack_.back().isTerminal) {
                bool reduced = false;
                for (int i = (int)stack_.size() - 1; i >= 1; i--) {
                    if (!stack_[i].isTerminal) {
                        vector<string> cand = {stack_[i].symbol};
                        for (const auto& prod : grammar_.getProductions()) {
                            if (prod.right.size() == 1 && prod.right[0] == cand[0]) {
                                // 上下文敏感：S → E 不在括号内归约
                                if (prod.left == "S" && prod.right[0] == "E" && i > 0) {
                                    auto& prev = stack_[i - 1];
                                    if (prev.isTerminal) {
                                        string p = prev.symbol;
                                        if (p == "(") {
                                            continue;
                                        }
                                    }
                                }
                                int line = stack_[i].line;
                                semanticAction(prod, i);
                                buildASTForProduction(prod, i);
                                stack_.erase(stack_.begin() + i);
                                stack_.insert(stack_.begin() + i,
                                    StackSymbol{prod.left, false, line, createNode(prod.left, false, line), ""});
                                reduced = true;
                                break;
                            }
                        }
                        if (reduced) break;
                    }
                }
                if (reduced) continue;
            }
            push({currentTerminal, true, (int)currentToken.line,
                  createNode(currentTerminal, true, (int)currentToken.line), currentToken.value});
            inputPos++;
        } else if (relation == ">") {
            // 特殊处理：id 后跟 . id 时，移进 . 而非归约 id → E
            // 这样 p.x 可以正确识别为成员访问
            if (currentTerminal == "." && stack_.size() >= 1 &&
                stack_.back().isTerminal && stack_.back().symbol == "id" &&
                inputPos + 1 < tokens.size() && tokenToTerminal(tokens[inputPos + 1]) == "id") {
                push({currentTerminal, true, (int)currentToken.line,
                      createNode(currentTerminal, true, (int)currentToken.line), currentToken.value});
                trace_.push_back({stackToString(), "", "SHIFT: " + currentTerminal + " (member access)"});
                inputPos++;
                continue;
            }
            // 特殊处理：栈顶是 id . id 时，整体归约为 E（成员访问）
            if (stack_.size() >= 3 &&
                stack_.back().isTerminal && stack_.back().symbol == "id" &&
                stack_[stack_.size()-2].isTerminal && stack_[stack_.size()-2].symbol == "." &&
                stack_[stack_.size()-3].isTerminal && stack_[stack_.size()-3].symbol == "id") {
                int line = stack_.back().line;
                string memberAddr = stack_.back().addr;
                string baseAddr = stack_[stack_.size()-3].addr;
                sem_.checkMemberAccess(baseAddr, memberAddr, line);
                // 构建 AST
                auto memberNode = make_shared<ASTNode>(ASTNodeType::LValueMember, memberAddr, line);
                memberNode->addChild(make_shared<ASTNode>(ASTNodeType::IdLeaf, baseAddr, stack_[stack_.size()-3].line));
                stack_.pop_back(); stack_.pop_back(); stack_.pop_back();
                string temp = sem_.newTemp();
                sem_.emit(".", baseAddr, memberAddr, temp);
                int nodeId = createNode("E", false, line);
                push({"E", false, line, nodeId, temp, "", 0, memberNode});
                trace_.push_back({stackToString(), "", "REDUCE: id . id → E (member access)"});
                continue;
            }
            if (!reduce()) {
                // > 关系但无法归约 → 尝试归约栈中的非终结符
                // 处理 E ; 的情况：先归约 E → S，再归约 S ; → P
                bool reduced = false;
                for (int i = (int)stack_.size() - 1; i >= 1; i--) {
                    if (!stack_[i].isTerminal) {
                        // 检查前一个符号是否是运算符，如果是则不归约
                        // 运算符后的非终结符是表达式的一部分
                        if (i >= 1 && stack_[i-1].isTerminal) {
                            string prev = stack_[i-1].symbol;
                            if (prev == "+" || prev == "-" || prev == "*" || prev == "/" ||
                                prev == "<" || prev == ">" || prev == "==" || prev == "=" ||
                                prev == "." || prev == "[") {
                                continue;
                            }
                        }
                        vector<string> cand = {stack_[i].symbol};
                        for (const auto& prod : grammar_.getProductions()) {
                            if (prod.right.size() == 1 && prod.right[0] == cand[0]) {
                                // 上下文敏感：S → E 不在括号内归约
                                if (prod.left == "S" && prod.right[0] == "E" && i > 0) {
                                    auto& prev = stack_[i - 1];
                                    if (prev.isTerminal) {
                                        string p = prev.symbol;
                                        if (p == "(") {
                                            continue;
                                        }
                                    }
                                }
                                int line = stack_[i].line;
                                semanticAction(prod, i);
                                buildASTForProduction(prod, i);
                                stack_.erase(stack_.begin() + i);
                                stack_.insert(stack_.begin() + i,
                                    StackSymbol{prod.left, false, line, createNode(prod.left, false, line), ""});
                                reduced = true;
                                break;
                            }
                        }
                        if (reduced) break;
                    }
                }
                if (!reduced) {
                    hasSyntaxError = true;
                    errorHandler_.reportError(currentToken.line, currentToken.column,
                        "Syntax error: cannot reduce near '" + currentToken.value + "'");
                    inputPos++;
                }
            }
        } else {
            // 无关系 → 尝试归约
            if (!reduce()) {
                // 归约失败 → 特殊处理：栈顶是非终结符且输入是 ; 或 $ 时
                // 尝试归约栈顶非终结符（如 E → S）
                if (!stack_.empty() && !stack_.back().isTerminal &&
                    (currentTerminal == ";" || currentTerminal == "$")) {
                    // 从栈顶往下找非终结符进行归约
                    for (int i = (int)stack_.size() - 1; i >= 1; i--) {
                        if (!stack_[i].isTerminal) {
                            vector<string> cand = {stack_[i].symbol};
                            bool found = false;
                            for (const auto& prod : grammar_.getProductions()) {
                                if (prod.right.size() == 1 && prod.right[0] == cand[0]) {
                                    // 上下文敏感：S → E 不在括号内归约
                                    if (prod.left == "S" && prod.right[0] == "E" && i > 0) {
                                        auto& prev = stack_[i - 1];
                                        if (prev.isTerminal) {
                                            string p = prev.symbol;
                                            if (p == "(") {
                                                continue;
                                            }
                                        }
                                    }
                                    int line = stack_[i].line;
                                    semanticAction(prod, i);
                                    buildASTForProduction(prod, i);
                                    stack_.erase(stack_.begin() + i);
                                    stack_.insert(stack_.begin() + i,
                                        StackSymbol{prod.left, false, line, createNode(prod.left, false, line), ""});
                                    found = true;
                                    break;
                                }
                            }
                            if (found) break;
                        }
                    }
                    continue;
                }
                hasSyntaxError = true;
                errorHandler_.reportError(currentToken.line, currentToken.column,
                    "Syntax error near '" + currentToken.value + "'");
                inputPos++;
            }
        }
    }

    // 输入结束，归约剩余
    while (true) {
        if (stack_.size() == 2 && stack_[0].symbol == "$" &&
            stack_[1].symbol == grammar_.getStartSymbol()) {
            astRoot_ = stack_[1].astNode;
            return !hasSyntaxError;
        }
        if (!reduce()) {
            // P → S ; 归约
            if (stack_.size() >= 3 && stack_.back().symbol == ";" &&
                stack_[stack_.size()-2].symbol == "S") {
                for (const auto& prod : grammar_.getProductions()) {
                    if (prod.left == grammar_.getStartSymbol() &&
                        prod.right.size() == 2 && prod.right[0] == "S" && prod.right[1] == ";") {
                        int line = stack_.back().line;
                        auto sNode = stack_[stack_.size()-2].astNode;
                        auto progNode = make_shared<ASTNode>(ASTNodeType::Program, "", line);
                        if (sNode) progNode->addChild(sNode);
                        stack_.pop_back(); stack_.pop_back();
                        push({prod.left, false, line, createNode(prod.left, false, line), "", "", 0, progNode});
                        goto next_end;
                    }
                }
            }
            if (!hasSyntaxError) {
                errorHandler_.reportError(0, 0, "Syntax error: unexpected end of input");
            }
            return false;
        }
        next_end:;
    }
}

// ==================== 输出 ====================
void PrecedenceParser::dumpTrace(const string& filename) const {
    ofstream f(filename);
    f << "========== 移进-归约过程 ==========\n\n";
    f << "Step\tStack\t\t\t\tAction\n";
    f << "----\t----\t\t\t\t------\n";
    for (size_t i = 0; i < trace_.size(); i++) {
        f << (i + 1) << "\t" << trace_[i].stackStr << "\t\t" << trace_[i].action << "\n";
    }
}

void PrecedenceParser::dumpParseTree(const string& filename) const {
    ofstream f(filename);
    f << "========== 语法分析树 ==========\n\n";

    int rootId = -1;
    for (int i = (int)treeNodes_.size() - 1; i >= 0; i--) {
        if (treeNodes_[i].symbol == grammar_.getStartSymbol() && !treeNodes_[i].isTerminal) {
            rootId = i;
            break;
        }
    }

    if (rootId < 0) { f << "(无法构建语法树)\n"; return; }

    std::function<void(int, int)> printNode = [&](int nodeId, int depth) {
        const TreeNode& node = treeNodes_[nodeId];
        for (int i = 0; i < depth; i++) f << "│  ";
        if (node.isTerminal) {
            f << "├─ " << node.symbol;
            if (node.line > 0) f << "  (line " << node.line << ")";
            f << "\n";
        } else {
            f << "├─ " << node.symbol << "\n";
            for (int childId : node.children) printNode(childId, depth + 1);
        }
    };
    printNode(rootId, 0);
}

void PrecedenceParser::dumpAll() const {
    dumpTrace("output/Parser/trace.txt");
    dumpParseTree("output/Parser/parse_tree.txt");
}
