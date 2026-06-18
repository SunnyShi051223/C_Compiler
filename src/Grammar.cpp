#include "../include/Grammar.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using std::string;
using std::vector;
using std::set;
using std::map;
using std::ofstream;
using std::endl;

Grammar::Grammar() {
    initProductions();
    initSymbols();
    computeFirstVT();
    computeLastVT();
    buildPrecedenceTable();
}

// ========================================================
// 简单优先文法（增强版）
//
// 新增支持:
// - E → E > E   (大于运算符)
// - S → int id [ num ]  (数组声明)
// - S → id . id = E     (成员访问赋值)
// - S → id [ E ] = E    (数组元素赋值)
// ========================================================

void Grammar::initProductions() {
    int id = 1;

    // 程序结构
    productions_.push_back({id++, "P", {"S", ";", "P"}});
    productions_.push_back({id++, "P", {"S", ";"}});

    // 声明
    productions_.push_back({id++, "S", {"int", "id"}});
    productions_.push_back({id++, "S", {"char", "id"}});
    productions_.push_back({id++, "S", {"double", "id"}});
    productions_.push_back({id++, "S", {"struct", "id", "{", "P", "}"}});
    productions_.push_back({id++, "S", {"struct", "id", "id"}});

    // 赋值语句
    productions_.push_back({id++, "S", {"id", "=", "E"}});
    productions_.push_back({id++, "S", {"id", ".", "id", "=", "E"}});
    productions_.push_back({id++, "S", {"id", "[", "E", "]", "=", "E"}});

    // 函数调用
    productions_.push_back({id++, "S", {"id", "(", "E", ")"}});
    productions_.push_back({id++, "S", {"id", "(", "id", ")"}});
    productions_.push_back({id++, "S", {"id", "(", ")"}});

    // 控制流
    productions_.push_back({id++, "S", {"while", "(", "E", ")", "{", "P", "}"}});
    productions_.push_back({id++, "S", {"if", "(", "E", ")", "{", "P", "}", "else", "{", "P", "}"}});
    productions_.push_back({id++, "S", {"return", "E"}});

    // 表达式
    productions_.push_back({id++, "E", {"E", "+", "E"}});
    productions_.push_back({id++, "E", {"E", "-", "E"}});
    productions_.push_back({id++, "E", {"E", "*", "E"}});
    productions_.push_back({id++, "E", {"E", "/", "E"}});
    productions_.push_back({id++, "E", {"E", "<", "E"}});
    productions_.push_back({id++, "E", {"E", ">", "E"}});
    productions_.push_back({id++, "E", {"E", "==", "E"}});
    productions_.push_back({id++, "E", {"E", ".", "id"}});
    productions_.push_back({id++, "E", {"E", "[", "E", "]"}});
    productions_.push_back({id++, "E", {"(", "E", ")"}});
    productions_.push_back({id++, "E", {"id"}});
    productions_.push_back({id++, "E", {"num"}});
    productions_.push_back({id++, "E", {"char"}});

    startSymbol_ = "P";
}

void Grammar::initSymbols() {
    set<string> allSymbols;
    for (const auto& prod : productions_) {
        allSymbols.insert(prod.left);
        for (const auto& s : prod.right) allSymbols.insert(s);
    }
    for (const auto& prod : productions_) nonTerminals_.insert(prod.left);
    for (const auto& s : allSymbols) {
        if (nonTerminals_.find(s) == nonTerminals_.end()) terminals_.insert(s);
    }
    terminals_.insert("$");
}

void Grammar::computeFirstVT() {
    for (const auto& nt : nonTerminals_) firstVT_[nt] = set<string>();
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& prod : productions_) {
            const string& A = prod.left;
            const auto& rhs = prod.right;
            if (rhs.empty()) continue;
            if (isTerminal(rhs[0]))
                if (firstVT_[A].insert(rhs[0]).second) changed = true;
            if (rhs.size() >= 2 && isNonTerminal(rhs[0]) && isTerminal(rhs[1]))
                if (firstVT_[A].insert(rhs[1]).second) changed = true;
            if (isNonTerminal(rhs[0]))
                for (const auto& a : firstVT_[rhs[0]])
                    if (firstVT_[A].insert(a).second) changed = true;
        }
    }
}

void Grammar::computeLastVT() {
    for (const auto& nt : nonTerminals_) lastVTMap_[nt] = set<string>();
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& prod : productions_) {
            const string& A = prod.left;
            const auto& rhs = prod.right;
            if (rhs.empty()) continue;
            size_t last = rhs.size() - 1;
            if (isTerminal(rhs[last]))
                if (lastVTMap_[A].insert(rhs[last]).second) changed = true;
            if (rhs.size() >= 2 && isTerminal(rhs[last - 1]) && isNonTerminal(rhs[last]))
                if (lastVTMap_[A].insert(rhs[last - 1]).second) changed = true;
            if (isNonTerminal(rhs[last]))
                for (const auto& a : lastVTMap_[rhs[last]])
                    if (lastVTMap_[A].insert(a).second) changed = true;
        }
    }
}

void Grammar::buildPrecedenceTable() {
    for (const auto& a : terminals_)
        for (const auto& b : terminals_) relation_[a][b] = "";

    for (const auto& prod : productions_) {
        const auto& rhs = prod.right;
        for (size_t i = 0; i + 1 < rhs.size(); i++) {
            const string& Xi = rhs[i];
            const string& Xi1 = rhs[i + 1];
            if (isTerminal(Xi) && isTerminal(Xi1))
                relation_[Xi][Xi1] = "=";
            if (isTerminal(Xi) && isNonTerminal(Xi1))
                for (const auto& a : firstVT_[Xi1])
                    if (relation_[Xi][a].empty() || relation_[Xi][a] == "<")
                        relation_[Xi][a] = "<";
            if (isNonTerminal(Xi) && isTerminal(Xi1))
                for (const auto& a : lastVTMap_[Xi])
                    if (relation_[a][Xi1].empty() || relation_[a][Xi1] == ">")
                        relation_[a][Xi1] = ">";
        }
    }

    for (const auto& a : firstVT_[startSymbol_]) relation_["$"][a] = "<";
    for (const auto& a : lastVTMap_[startSymbol_]) relation_[a]["$"] = ">";

    // 括号匹配特殊关系
    relation_["("][")"] = "=";
    relation_["["]["]"] = "=";
    relation_["{"]["}"] = "=";

    // 修复 FIRSTVT 传播导致的 = < ; 问题
    // 在 S → id = E ; 中，正确关系应为 = > ;（归约 id = E → S）
    relation_["="][";"] = ">";
}

const vector<Production>& Grammar::getProductions() const { return productions_; }
bool Grammar::isTerminal(const string& s) const { return terminals_.count(s); }
bool Grammar::isNonTerminal(const string& s) const { return nonTerminals_.count(s); }
const set<string>& Grammar::getFirstVT(const string& nt) const {
    static const set<string> empty;
    auto it = firstVT_.find(nt);
    return it != firstVT_.end() ? it->second : empty;
}
const set<string>& Grammar::getLastVT(const string& nt) const {
    static const set<string> empty;
    auto it = lastVTMap_.find(nt);
    return it != lastVTMap_.end() ? it->second : empty;
}
string Grammar::getRelation(const string& a, const string& b) const {
    auto itA = relation_.find(a);
    if (itA == relation_.end()) return "";
    auto itB = itA->second.find(b);
    return itB != itA->second.end() ? itB->second : "";
}
const set<string>& Grammar::getTerminals() const { return terminals_; }
const set<string>& Grammar::getNonTerminals() const { return nonTerminals_; }
const string& Grammar::getStartSymbol() const { return startSymbol_; }

void Grammar::dumpProductions(const string& filename) const {
    ofstream f(filename);
    f << "========== 产生式列表 ==========\n\n";
    for (const auto& p : productions_) {
        f << "(" << p.id << ")  " << p.left << " → ";
        for (const auto& s : p.right) f << s << " ";
        f << "\n";
    }
}
void Grammar::dumpFirstVT(const string& filename) const {
    ofstream f(filename);
    f << "========== FIRSTVT 集合 ==========\n\n";
    for (const auto& nt : nonTerminals_) {
        f << "FIRSTVT(" << nt << ") = { ";
        bool first = true;
        for (const auto& sym : firstVT_.at(nt)) { if (!first) f << ", "; f << sym; first = false; }
        f << " }\n";
    }
}
void Grammar::dumpLastVT(const string& filename) const {
    ofstream f(filename);
    f << "========== LASTVT 集合 ==========\n\n";
    for (const auto& nt : nonTerminals_) {
        f << "LASTVT(" << nt << ") = { ";
        bool first = true;
        for (const auto& sym : lastVTMap_.at(nt)) { if (!first) f << ", "; f << sym; first = false; }
        f << " }\n";
    }
}
void Grammar::dumpPrecedenceTable(const string& filename) const {
    ofstream f(filename);
    f << "========== 优先关系表 ==========\n\n";
    vector<string> terms(terminals_.begin(), terminals_.end());
    f << "\t";
    for (const auto& t : terms) f << t << "\t";
    f << "\n";
    for (const auto& a : terms) {
        f << a << "\t";
        for (const auto& b : terms) {
            string rel = getRelation(a, b);
            f << (rel.empty() ? " " : rel) << "\t";
        }
        f << "\n";
    }
}
void Grammar::dumpAll() const {
    dumpProductions("productions.txt");
    dumpFirstVT("firstvt.txt");
    dumpLastVT("lastvt.txt");
    dumpPrecedenceTable("precedence_table.txt");
}
