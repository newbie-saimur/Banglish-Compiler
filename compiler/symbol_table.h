#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct Symbol {
    std::string name;
    std::string dtype;
    int line;
    bool initialized;
};

struct SymbolTable {
    std::unordered_map<std::string, Symbol> table;
    std::vector<std::string> order;
    void declare(const std::string& name, const std::string& dtype, int line){
        if(!table.count(name)) order.push_back(name);
        table[name] = {name,dtype,line,false};
    }
    void initialize(const std::string& name){ if(table.count(name)) table[name].initialized=true; }
    std::vector<Symbol> all() const{
        std::vector<Symbol> v; v.reserve(order.size());
        for(auto &n: order){ auto it=table.find(n); if(it!=table.end()) v.push_back(it->second); }
        return v;
    }
};
