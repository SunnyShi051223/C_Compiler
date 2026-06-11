#pragma once

#include <string>
#include <unordered_map>

// --- 符号表条目 ---
struct SymbolInfo {
    std::string name;
};

// --- 符号表 ---
class SymbolTable {
public:
    void insert(const std::string& name);
    bool lookup(const std::string& name) const;

private:
    std::unordered_map<std::string, SymbolInfo> table;
};
