#include "../include/PrecedenceParser.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

using std::string;
using std::vector;
using std::ofstream;
using std::cerr;
using std::endl;
using std::to_string;

PrecedenceParser::PrecedenceParser(const Grammar& grammar, ErrorHandler& errorHandler,
                                   SemanticAnalyzer& semAnalyzer)
    : grammar_(grammar), errorHandler_(errorHandler), sem_(semAnalyzer), nextNodeId_(0) {}

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

    // --- 声明语句: T id ---
    if (len == 3 && rhs[0] == "struct" && rhs[1] == "id" && rhs[2] == "id") {
        // S → struct id id : 声明结构体变量
        string structType = stack_[start + 1].addr;
        string varName = stack_[start + 2].addr;
        sem_.checkStructDefined(structType, stack_[start].line);
        sem_.declareVariable(varName, structType);
    }
    else if (len == 2 && (rhs[0] == "int" || rhs[0] == "char" || rhs[0] == "double")) {
        // S → int id / char id / double id
        string varName = stack_[start + 1].addr;
        if (sem_.inStructBody_ && !sem_.currentStructName_.empty()) {
            sem_.addStructMember(sem_.currentStructName_, rhs[0], varName);
        } else {
            sem_.declareVariable(varName, rhs[0]);
        }
    }

    // --- 结构体定义: struct id { P } ---
    else if (len == 5 && rhs[0] == "struct") {
        string structName = stack_[start + 1].addr;
        sem_.defineStruct(structName);
        sem_.inStructBody_ = false;
        sem_.currentStructName_ = "";
    }

    // --- 赋值语句: id = E ---
    else if (len == 3 && rhs[0] == "id" && rhs[1] == "=") {
        string varName = stack_[start].addr;
        string exprAddr = stack_[start + 2].addr;
        sem_.checkDeclared(varName, stack_[start].line);
        sem_.checkAssignment(varName, exprAddr, stack_[start].line);
        sem_.emit("=", exprAddr, "", varName);
    }

    // --- 成员赋值: id . id = E ---
    else if (len == 5 && rhs[0] == "id" && rhs[1] == "." && rhs[3] == "=") {
        string base = stack_[start].addr;
        string member = stack_[start + 2].addr;
        string exprAddr = stack_[start + 4].addr;
        sem_.checkMemberAccess(base, member, stack_[start].line);
        string memberAddr = base + "." + member;
        sem_.emit("=", exprAddr, "", memberAddr);
    }

    // --- 数组元素赋值: id [ E ] = E ---
    else if (len == 6 && rhs[0] == "id" && rhs[1] == "[" && rhs[3] == "]" && rhs[4] == "=") {
        string arrName = stack_[start].addr;
        string indexAddr = stack_[start + 2].addr;
        string exprAddr = stack_[start + 5].addr;
        string arrAddr = arrName + "[" + indexAddr + "]";
        sem_.emit("=", exprAddr, "", arrAddr);
    }

    // --- 函数调用: id ( E ) ---
    else if (len == 4 && rhs[0] == "id" && rhs[1] == "(" && rhs[2] == "E") {
        string funcName = stack_[start].addr;
        string argAddr = stack_[start + 2].addr;
        if (funcName == "write") {
            sem_.emit("param", argAddr, "", "");
            sem_.emit("call", "write", "1", "_");
            sem_.emit("call", "println", "0", "_");
        } else {
            sem_.emit("param", argAddr, "", "");
            sem_.emit("call", funcName, "1", "_");
        }
    }

    // --- 函数调用: id ( id ) ---
    else if (len == 4 && rhs[0] == "id" && rhs[1] == "(" && rhs[2] == "id") {
        string funcName = stack_[start].addr;
        string argName = stack_[start + 2].addr;
        if (funcName == "read") {
            sem_.emit("call", "read", "0", argName);
        } else {
            sem_.emit("param", argName, "", "");
            sem_.emit("call", funcName, "1", "_");
        }
    }

    // --- 函数调用: id ( ) ---
    else if (len == 3 && rhs[0] == "id" && rhs[1] == "(" && rhs[2] == ")") {
        string funcName = stack_[start].addr;
        sem_.emit("call", funcName, "0", "_");
    }

    // --- 循环语句: while ( E ) { P } ---
    else if (len == 7 && rhs[0] == "while") {
        string condAddr = stack_[start + 2].addr;
        string L0 = sem_.newLabel();
        string L1 = sem_.newLabel();

        vector<Quadruple> condQuads = sem_.extractLastQuads(1);
        sem_.emit("label", L0, "", "");
        sem_.restoreQuads(condQuads);
        int jumpId = sem_.emit("if_false", condAddr, "", "");
        sem_.emit("goto", L0, "", "");
        sem_.backpatch(jumpId, L1);
        sem_.emit("label", L1, "", "");
    }

    // --- 选择语句: if ( E ) { P } else { P } ---
    else if (len == 11 && rhs[0] == "if") {
        string condAddr = stack_[start + 2].addr;
        string L1 = sem_.newLabel();
        string L2 = sem_.newLabel();

        int jumpId = sem_.emit("if_false", condAddr, "", "");
        int gotoId = sem_.emit("goto", L2, "", "");
        sem_.backpatch(jumpId, L1);
        sem_.emit("label", L1, "", "");
        sem_.backpatch(gotoId, L2);
        sem_.emit("label", L2, "", "");
    }

    // --- 返回语句: return E ---
    else if (len == 2 && rhs[0] == "return") {
        string retAddr = stack_[start + 1].addr;
        sem_.emit("return", retAddr, "", "");
    }

    // --- 算术/关系表达式: E op E ---
    else if (len == 3 && rhs[0] == "E" && rhs[2] == "E") {
        string left = stack_[start].addr;
        string op = stack_[start + 1].symbol;
        string right = stack_[start + 2].addr;
        string temp = sem_.newTemp();
        sem_.emit(op, left, right, temp);
        stack_[start].addr = temp;
    }

    // --- 成员访问: E . id ---
    else if (len == 3 && rhs[0] == "E" && rhs[1] == ".") {
        string base = stack_[start].addr;
        string member = stack_[start + 2].addr;
        string temp = sem_.newTemp();
        sem_.emit(".", base, member, temp);
        stack_[start].addr = temp;
    }

    // --- 数组下标: E [ E ] ---
    else if (len == 4 && rhs[0] == "E" && rhs[1] == "[") {
        string base = stack_[start].addr;
        string index = stack_[start + 2].addr;
        string temp = sem_.newTemp();
        sem_.emit("[]", base, index, temp);
        stack_[start].addr = temp;
    }

    // --- 括号表达式: ( E ) ---
    else if (len == 3 && rhs[0] == "(" && rhs[2] == ")") {
        stack_[start].addr = stack_[start + 1].addr;
    }

    // --- 基本元素: id / num / char ---
    else if (len == 1 && rhs[0] == "id") {
        string name = stack_[start].addr;
        if (!sem_.isDeclared(name)) {
            sem_.checkDeclared(name, stack_[start].line);
            // 隐式声明（容错）
            sem_.declareVariable(name, "int");
        }
    }
    // E → num / E → char : addr 已在移进时设置
}

// ==================== 归约 ====================
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
                    int line = 0;
                    for (int i = start; i < (int)stack_.size(); i++)
                        if (stack_[i].isTerminal && stack_[i].line > 0) line = stack_[i].line;

                    // 执行语义动作
                    semanticAction(prod, start);

                    // 收集子节点 ID
                    vector<int> childIds;
                    for (int i = start; i < (int)stack_.size(); i++)
                        childIds.push_back(stack_[i].treeNodeId);

                    // 保留语义值
                    string addr = stack_[start].addr;

                    // 创建归约节点
                    int nodeId = createNode(prod.left, false, line);
                    treeNodes_[nodeId].children = childIds;

                    // 弹出右部，压入左部
                    for (int i = 0; i < len; i++) stack_.pop_back();
                    StackSymbol sym;
                    sym.symbol = prod.left;
                    sym.isTerminal = false;
                    sym.line = line;
                    sym.treeNodeId = nodeId;
                    sym.addr = addr;
                    push(sym);

                    // 记录日志
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
    return false;
}

// ==================== 主解析函数 ====================
bool PrecedenceParser::parse(const vector<Token>& tokens) {
    stack_.clear();
    treeNodes_.clear();
    trace_.clear();
    nextNodeId_ = 0;

    int dollarNodeId = createNode("$", true, 0);
    StackSymbol dollarSym;
    dollarSym.symbol = "$";
    dollarSym.isTerminal = true;
    dollarSym.line = 0;
    dollarSym.treeNodeId = dollarNodeId;
    push(dollarSym);

    size_t inputPos = 0;
    bool hasSyntaxError = false;

    auto inputToString = [&](size_t pos) -> string {
        string s;
        for (size_t i = pos; i < tokens.size(); i++) {
            if (i > pos) s += " ";
            s += tokenToTerminal(tokens[i]);
        }
        return s;
    };

    while (inputPos < tokens.size()) {
        Token currentToken = tokens[inputPos];
        string currentTerminal = tokenToTerminal(currentToken);

        if (currentToken.type == TokenType::UNKNOWN) { inputPos++; continue; }

        if (currentTerminal == "$" && stack_.size() == 2 &&
            stack_[0].symbol == "$" && stack_[1].symbol == grammar_.getStartSymbol()) {
            trace_.push_back({stackToString(), inputToString(inputPos), "ACCEPT"});
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

        // 特殊处理：栈顶是 id . id 时，处理成员访问/赋值
        if (stack_.size() >= 3 &&
            stack_.back().isTerminal && stack_.back().symbol == "id" &&
            stack_[stack_.size()-2].isTerminal && stack_[stack_.size()-2].symbol == "." &&
            stack_[stack_.size()-3].isTerminal && stack_[stack_.size()-3].symbol == "id") {
            if (currentTerminal == "=") {
                // 成员赋值：移进 =
                int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
                StackSymbol sym;
                sym.symbol = currentTerminal;
                sym.isTerminal = true;
                sym.line = (int)currentToken.line;
                sym.treeNodeId = nodeId;
                sym.addr = currentToken.value;
                push(sym);
                trace_.push_back({stackToString(), inputToString(inputPos + 1),
                    "SHIFT: " + currentTerminal + " (member assign)"});
                inputPos++;
                continue;
            } else {
                // 成员访问：归约 id . id → E
                int line = stack_.back().line;
                string memberAddr = stack_.back().addr;
                string baseAddr = stack_[stack_.size()-3].addr;
                sem_.checkMemberAccess(baseAddr, memberAddr, line);
                stack_.pop_back(); stack_.pop_back(); stack_.pop_back();
                string temp = sem_.newTemp();
                sem_.emit(".", baseAddr, memberAddr, temp);
                int nodeId = createNode("E", false, line);
                StackSymbol sym2;
                sym2.symbol = "E"; sym2.isTerminal = false;
                sym2.line = line; sym2.treeNodeId = nodeId; sym2.addr = temp;
                push(sym2);
                trace_.push_back({stackToString(), inputToString(inputPos),
                    "REDUCE: id . id → E (member access)"});
                continue;
            }
        }

        // 特殊处理：当标准算法找不到关系时，尝试归约赋值语句
        // 解决 = 和 ; 之间无优先关系的问题（如 id = arr[i] ;）
        if (currentTerminal == ";" && relation.empty() &&
            stack_.size() >= 3 && stack_.back().symbol == "E" && !stack_.back().isTerminal) {
            // 从栈顶往下找 id = E 模式
            for (int i = (int)stack_.size() - 2; i >= 1; i--) {
                if (stack_[i].isTerminal && stack_[i].symbol == "=" &&
                    stack_[i-1].isTerminal && stack_[i-1].symbol == "id") {
                    int start = i - 1;
                    int line = stack_[start].line;
                    string varName = stack_[start].addr;
                    string exprAddr = stack_.back().addr;
                    // 成员赋值：id . id = E
                    if (start >= 2 && stack_[start-1].isTerminal && stack_[start-1].symbol == "." &&
                        stack_[start-2].isTerminal && stack_[start-2].symbol == "id") {
                        string base = stack_[start-2].addr;
                        string member = stack_[start].addr;
                        sem_.emit("=", exprAddr, "", base + "." + member);
                    } else {
                        sem_.emit("=", exprAddr, "", varName);
                    }
                    int popCount = (int)stack_.size() - start;
                    vector<int> childIds;
                    for (int j = start; j < (int)stack_.size(); j++)
                        childIds.push_back(stack_[j].treeNodeId);
                    for (int j = 0; j < popCount; j++) stack_.pop_back();
                    int nodeId = createNode("S", false, line);
                    treeNodes_[nodeId].children = childIds;
                    StackSymbol sym;
                    sym.symbol = "S"; sym.isTerminal = false;
                    sym.line = line; sym.treeNodeId = nodeId; sym.addr = varName;
                    push(sym);
                    trace_.push_back({stackToString(), inputToString(inputPos),
                        "REDUCE: ... = E → S (assign)"});
                    continue;
                }
            }
        }

        // 特殊处理：栈顶是 id 且后跟 [ 时，不归约 id → E
        if (currentTerminal == "[" && stack_.size() >= 1 &&
            stack_.back().isTerminal && stack_.back().symbol == "id") {
            int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
            StackSymbol sym;
            sym.symbol = currentTerminal; sym.isTerminal = true;
            sym.line = (int)currentToken.line; sym.treeNodeId = nodeId;
            sym.addr = currentToken.value;
            push(sym);
            trace_.push_back({stackToString(), inputToString(inputPos + 1),
                "SHIFT: " + currentTerminal + " (array index)"});
            inputPos++;
            continue;
        }

        // 特殊处理：栈顶是 [ E 且后跟 ] 时，移进 ]
        if (currentTerminal == "]" && stack_.size() >= 2 &&
            stack_.back().symbol == "E" && !stack_.back().isTerminal &&
            stack_[stack_.size()-2].isTerminal && stack_[stack_.size()-2].symbol == "[") {
            int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
            StackSymbol sym;
            sym.symbol = currentTerminal; sym.isTerminal = true;
            sym.line = (int)currentToken.line; sym.treeNodeId = nodeId;
            sym.addr = currentToken.value;
            push(sym);
            trace_.push_back({stackToString(), inputToString(inputPos + 1),
                "SHIFT: " + currentTerminal + " (array close)"});
            inputPos++;
            continue;
        }


        // 特殊处理：栈中有 id = [...] E 且 = 和 E 不相邻时（如 arr[i] 归约后）
        // 只在 = 紧跟 id（无其他表达式运算符）时触发
        if (currentTerminal == ";" && stack_.size() >= 4 &&
            stack_.back().symbol == "E" && !stack_.back().isTerminal &&
            stack_[stack_.size()-2].symbol != "=") {
            for (int i = (int)stack_.size() - 2; i >= 1; i--) {
                if (stack_[i].isTerminal && stack_[i].symbol == "=" &&
                    stack_[i-1].isTerminal && stack_[i-1].symbol == "id" &&
                    i < (int)stack_.size() - 2) {
                    // 检查 = 和 id 之间是否有运算符（+ - * / < > ==）
                    bool hasOperator = false;
                    for (int j = i + 1; j < (int)stack_.size() - 1; j++) {
                        if (stack_[j].isTerminal && (stack_[j].symbol == "+" ||
                            stack_[j].symbol == "-" || stack_[j].symbol == "*" ||
                            stack_[j].symbol == "/" || stack_[j].symbol == "<" ||
                            stack_[j].symbol == ">" || stack_[j].symbol == "==")) {
                            hasOperator = true;
                            break;
                        }
                    }
                    if (hasOperator) break; // 有运算符，让标准算法处理

                    // 无运算符（如 id = [ E ] → id = E），特殊处理
                    int start = i - 1;
                    int line = stack_[start].line;
                    string varName = stack_[start].addr;
                    string exprAddr = stack_.back().addr;
                    if (start >= 2 && stack_[start-1].isTerminal && stack_[start-1].symbol == "." &&
                        stack_[start-2].isTerminal && stack_[start-2].symbol == "id") {
                        sem_.emit("=", exprAddr, "", stack_[start-2].addr + "." + varName);
                    } else {
                        sem_.emit("=", exprAddr, "", varName);
                    }
                    int popCount = (int)stack_.size() - start;
                    vector<int> childIds;
                    for (int j = start; j < (int)stack_.size(); j++)
                        childIds.push_back(stack_[j].treeNodeId);
                    for (int j = 0; j < popCount; j++) stack_.pop_back();
                    int nodeId = createNode("S", false, line);
                    treeNodes_[nodeId].children = childIds;
                    StackSymbol sym;
                    sym.symbol = "S"; sym.isTerminal = false;
                    sym.line = line; sym.treeNodeId = nodeId; sym.addr = varName;
                    push(sym);
                    continue;
                }
                if (stack_[i].symbol == ";" || stack_[i].symbol == "$") break;
            }
        }

        // 特殊处理：栈顶是 [ E ] 时
        if (stack_.size() >= 3 &&
            stack_.back().isTerminal && stack_.back().symbol == "]" &&
            stack_[stack_.size()-2].symbol == "E" &&
            stack_[stack_.size()-3].isTerminal && stack_[stack_.size()-3].symbol == "[") {

            // 检查是否为数组元素赋值（id [ E ] = E）
            // 只有当 [ 前面紧跟着 id 时才是数组赋值（如 a[0]=5）
            // 如果 [ 前面是 =，则是 x = arr[i] 中的表达式部分
            if (currentTerminal == "=" && stack_.size() >= 4 &&
                stack_[stack_.size()-4].isTerminal && stack_[stack_.size()-4].symbol == "id" &&
                !(stack_.size() >= 5 && stack_[stack_.size()-5].isTerminal && stack_[stack_.size()-5].symbol == "=")) {
                int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
                StackSymbol sym;
                sym.symbol = currentTerminal; sym.isTerminal = true;
                sym.line = (int)currentToken.line; sym.treeNodeId = nodeId;
                sym.addr = currentToken.value;
                push(sym);
                trace_.push_back({stackToString(), inputToString(inputPos + 1),
                    "SHIFT: " + currentTerminal + " (array assign)"});
                inputPos++;
                continue;
            }

            // 归约 [ E ] → E（数组下标表达式）
            int line = stack_.back().line;
            string indexAddr = stack_[stack_.size()-2].addr;
            string arrAddr = "arr";
            if (stack_.size() >= 4 && stack_[stack_.size()-4].isTerminal &&
                stack_[stack_.size()-4].symbol == "id") {
                arrAddr = stack_[stack_.size()-4].addr;
            }
            stack_.pop_back(); stack_.pop_back(); stack_.pop_back();
            string temp = sem_.newTemp();
            sem_.emit("[]", arrAddr, indexAddr, temp);
            int nodeId = createNode("E", false, line);
            StackSymbol sym;
            sym.symbol = "E"; sym.isTerminal = false;
            sym.line = line; sym.treeNodeId = nodeId; sym.addr = temp;
            push(sym);
            trace_.push_back({stackToString(), inputToString(inputPos),
                "REDUCE: [ E ] → E (array access)"});
            continue;
        }

        if (relation == "<" || relation == "=") {
            // 移进
            int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
            StackSymbol sym;
            sym.symbol = currentTerminal;
            sym.isTerminal = true;
            sym.line = (int)currentToken.line;
            sym.treeNodeId = nodeId;
            sym.addr = currentToken.value;  // 终结符的语义值为其字面值
            push(sym);
            trace_.push_back({stackToString(), inputToString(inputPos + 1),
                "SHIFT: " + currentTerminal});
            inputPos++;

            // 检测 struct id { 模式，设置结构体上下文
            if (currentTerminal == "{" && stack_.size() >= 3 &&
                stack_[stack_.size()-3].isTerminal && stack_[stack_.size()-3].symbol == "struct" &&
                stack_[stack_.size()-2].isTerminal && stack_[stack_.size()-2].symbol == "id") {
                sem_.inStructBody_ = true;
                sem_.currentStructName_ = stack_[stack_.size()-2].addr;
                sem_.defineStruct(sem_.currentStructName_);
            }
        } else if (relation == ">") {
            // 特殊处理：struct id id 模式
            if (currentTerminal == "id" && stack_.size() >= 2) {
                int topIdx = (int)stack_.size() - 1;
                if (stack_[topIdx].isTerminal && stack_[topIdx].symbol == "id" &&
                    stack_[topIdx - 1].isTerminal && stack_[topIdx - 1].symbol == "struct") {
                    int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
                    StackSymbol sym;
                    sym.symbol = currentTerminal;
                    sym.isTerminal = true;
                    sym.line = (int)currentToken.line;
                    sym.treeNodeId = nodeId;
                    sym.addr = currentToken.value;
                    push(sym);
                    trace_.push_back({stackToString(), inputToString(inputPos + 1),
                        "SHIFT: " + currentTerminal + " (struct id id)"});
                    inputPos++;
                    continue;
                }
            }
            // 特殊处理：id 后跟 . 且 . 后面还有 id 时（p.x 模式）
            // 不归约 id → E，而是移进 . 和 id，形成 id . id
            if (currentTerminal == "." && stack_.size() >= 1 &&
                stack_.back().isTerminal && stack_.back().symbol == "id" &&
                inputPos + 2 < tokens.size() && tokenToTerminal(tokens[inputPos + 1]) == "id") {
                int nodeId = createNode(currentTerminal, true, (int)currentToken.line);
                StackSymbol sym;
                sym.symbol = currentTerminal;
                sym.isTerminal = true;
                sym.line = (int)currentToken.line;
                sym.treeNodeId = nodeId;
                sym.addr = currentToken.value;
                push(sym);
                trace_.push_back({stackToString(), inputToString(inputPos + 1),
                    "SHIFT: " + currentTerminal + " (member access)"});
                inputPos++;
                continue;
            }
            if (!reduce()) {
                hasSyntaxError = true;
                errorHandler_.reportError(currentToken.line, currentToken.column,
                    "Syntax error: cannot reduce near '" + currentToken.value + "'");
                inputPos++;
            }
            if (stack_.size() == 2 && stack_[0].symbol == "$" &&
                stack_[1].symbol == grammar_.getStartSymbol() && currentTerminal == "$") {
                trace_.push_back({stackToString(), "", "ACCEPT"});
                return !hasSyntaxError;
            }
        } else {
            hasSyntaxError = true;
            errorHandler_.reportError(currentToken.line, currentToken.column,
                "Syntax error near '" + currentToken.value + "'");
            inputPos++;
        }
    }

    while (true) {
        if (stack_.size() == 2 && stack_[0].symbol == "$" &&
            stack_[1].symbol == grammar_.getStartSymbol()) {
            trace_.push_back({stackToString(), "", "ACCEPT"});
            return !hasSyntaxError;
        }
        if (!reduce()) {
            if (stack_.size() == 2 && stack_[0].symbol == "$" &&
                stack_[1].symbol == grammar_.getStartSymbol()) {
                trace_.push_back({stackToString(), "", "ACCEPT"});
                return !hasSyntaxError;
            }
            if (!hasSyntaxError) {
                errorHandler_.reportError(0, 0, "Syntax error: unexpected end of input");
            }
            return false;
        }
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

    if (rootId < 0) {
        f << "(无法构建语法树)\n";
        return;
    }

    std::function<void(int, int)> printNode = [&](int nodeId, int depth) {
        const TreeNode& node = treeNodes_[nodeId];
        for (int i = 0; i < depth; i++) f << "│  ";
        if (node.isTerminal) {
            f << "├─ " << node.symbol;
            if (node.line > 0) f << "  (line " << node.line << ")";
            f << "\n";
        } else {
            f << "├─ " << node.symbol << "\n";
            for (int childId : node.children) {
                printNode(childId, depth + 1);
            }
        }
    };

    printNode(rootId, 0);
}

void PrecedenceParser::dumpAll() const {
    dumpTrace("output/Parser/trace.txt");
    dumpParseTree("output/Parser/parse_tree.txt");
}
