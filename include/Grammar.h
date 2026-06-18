#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

// --- 产生式 ---
struct Production {
    int id;                 // 编号
    std::string left;       // 左部非终结符
    std::vector<std::string> right;  // 右部符号序列
};

// --- 文法类 ---
class Grammar {
public:
    Grammar();

    // 获取产生式列表
    const std::vector<Production>& getProductions() const;

    // 判断是否为终结符
    bool isTerminal(const std::string& symbol) const;

    // 判断是否为非终结符
    bool isNonTerminal(const std::string& symbol) const;

    // 获取 FIRSTVT / LASTVT 集合
    const std::set<std::string>& getFirstVT(const std::string& nonTerminal) const;
    const std::set<std::string>& getLastVT(const std::string& nonTerminal) const;

    // 获取优先关系: 返回 "<", "=", ">", 或 ""(无关系)
    // a 和 b 必须是终结符或 "$"
    std::string getRelation(const std::string& a, const std::string& b) const;

    // 获取所有终结符（含 "$"）
    const std::set<std::string>& getTerminals() const;

    // 获取所有非终结符
    const std::set<std::string>& getNonTerminals() const;

    // 获取开始符号
    const std::string& getStartSymbol() const;

    // 输出中间产物到文件
    void dumpProductions(const std::string& filename) const;
    void dumpFirstVT(const std::string& filename) const;
    void dumpLastVT(const std::string& filename) const;
    void dumpPrecedenceTable(const std::string& filename) const;

    // 输出所有中间产物
    void dumpAll() const;

private:
    std::vector<Production> productions_;
    std::set<std::string> terminals_;
    std::set<std::string> nonTerminals_;
    std::string startSymbol_;

    // FIRSTVT / LASTVT 集合
    std::map<std::string, std::set<std::string>> firstVT_;
    std::map<std::string, std::set<std::string>> lastVTMap_;

    // 优先关系表: relation_[a][b] = "<" / "=" / ">" / ""
    std::map<std::string, std::map<std::string, std::string>> relation_;

    // 初始化文法产生式
    void initProductions();

    // 初始化终结符和非终结符
    void initSymbols();

    // 计算 FIRSTVT 和 LASTVT 集合
    void computeFirstVT();
    void computeLastVT();

    // 构建优先关系表
    void buildPrecedenceTable();
};
