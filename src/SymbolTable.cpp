#include "../include/SymbolTable.h"

using std::string;
using std::unordered_map;

void SymbolTable::insert(const string& name) {
    if (table.find(name) == table.end()) {
        table[name] = SymbolInfo{name};
    }
}

bool SymbolTable::lookup(const string& name) const {
    return table.find(name) != table.end();
}
