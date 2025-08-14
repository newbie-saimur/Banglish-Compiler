#pragma once
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include "token.h"

namespace bg {

// Validate each token's lexeme against expected forms
inline std::vector<std::string> validateTokens(const std::vector<Token>& toks){
    std::vector<std::string> errors;
    std::regex reIdent(R"(^[A-Za-z_]\w*$)");
    std::regex reNumber(R"(^\d+(?:\.\d+)?$)");
    // strings are already quoted with escapes preserved
    std::regex reString(R"(^"(?:\\.|[^"])*"$)");
    std::regex reOp(R"(^(\+\+|--|\+=|-=|\*=|/=|<=|>=|==|!=|&&|\|\||[+\-*/%<>=!&|(){};,\[\]])$)");

    const std::unordered_set<std::string> kw = {
        // single-word
        "shuru","shesh","lekha","akkhor","sotto-mittha","jodi","nahoy","poro","dekhao","loop",
        // multi-word
        "purno sonkha","dosomik sonkha","ferot dao","nahoy jodi"
    };

    for(const auto& t: toks){
        if(t.type == "EOF") continue;
        const std::string &lex = t.lexeme;
        bool ok = true;
        if(t.type == "IDENT") ok = std::regex_match(lex, reIdent);
        else if(t.type == "NUMBER") ok = std::regex_match(lex, reNumber);
        else if(t.type == "STRING") ok = std::regex_match(lex, reString);
        else if(t.type == "OP") ok = std::regex_match(lex, reOp);
        else if(t.type == "KEYWORD") ok = kw.count(lex) > 0;
        if(!ok){
            errors.push_back("Token error at line " + std::to_string(t.line) + ", col " + std::to_string(t.col) + ": '" + lex + "' invalid for type " + t.type);
        }
    }
    return errors;
}

// Validate each non-empty line against simple statement regexes
inline std::vector<std::string> validateLines(const std::string& source){
    auto trim = [](const std::string& s){ size_t a=s.find_first_not_of(" \t\r\n"); if(a==std::string::npos) return std::string(); size_t b=s.find_last_not_of(" \t\r\n"); return s.substr(a,b-a+1); };
    // Declarations (with optional array size and optional initializer)
    std::regex reDecl(R"(^(purno sonkha|dosomik sonkha|lekha|akkhor|sotto-mittha)\s+[A-Za-z_]\w*(?:\s*\[\s*[^\]]+\s*\])?(?:\s*=\s*[^;]+)?\s*;\s*$)");
    std::regex reInput(R"(^poro\s*\(\s*[^)]+\)\s*;?\s*$)");
    std::regex rePrint(R"(^dekhao\s+.+;?\s*$)");
    std::regex reIf(R"(^jodi\s*\(.*\)\s*\{?\s*$)");
    std::regex reIfInline(R"(^jodi\s*\(.*\)\s*\{.*\}\s*$)");
    std::regex reElseIf(R"(^nahoy jodi\s*\(.*\)\s*\{?\s*$)");
    std::regex reElseIfInline(R"(^nahoy jodi\s*\(.*\)\s*\{.*\}\s*$)");
    std::regex reElse(R"(^nahoy(?:\s*\{?\s*)?$)");
    std::regex reReturn(R"(^ferot dao\s+[^;]+\s*;?\s*$)");
    std::regex reLoop(R"(^loop\s*\(.*;.*;.*\)\s*\{?\s*$)");
    std::regex reOnlyOpen(R"(^\{\s*$)");
    std::regex reOnlyClose(R"(^\}\s*$)");
    // Assignments and increments (with optional indexing)
    std::regex reAssign(R"(^\s*(?:\+\+|--)?\s*[A-Za-z_]\w*(?:\s*\[\s*[^\]]+\s*\])?\s*(?:\+\+|--|=(?:[^;]+)|\+=\s*[^;]+|-=\s*[^;]+|\*=\s*[^;]+|/=\s*[^;]+|%=\s*[^;]+)?\s*;?\s*$)");

    std::vector<std::string> errors;
    std::stringstream ss(source); std::string line; int ln=0;
    while(std::getline(ss, line)){
        ++ln; std::string L = trim(line);
        if(L.empty()) continue;
        if(L=="shuru" || L=="shesh") continue;
        // allow leading '}'s, as transpiler does
        while(!L.empty() && L[0]=='}'){
            L = trim(L.substr(1));
        }
        if(L.empty()) continue;
        bool match =
            std::regex_match(L, reDecl) ||
            std::regex_match(L, reInput) ||
            std::regex_match(L, rePrint) ||
            std::regex_match(L, reIf) ||
            std::regex_match(L, reIfInline) ||
            std::regex_match(L, reElseIf) ||
            std::regex_match(L, reElseIfInline) ||
            std::regex_match(L, reElse) ||
            std::regex_match(L, reReturn) ||
            std::regex_match(L, reLoop) ||
            std::regex_match(L, reOnlyOpen) ||
            std::regex_match(L, reOnlyClose) ||
            std::regex_match(L, reAssign);
        if(!match){
            errors.push_back("Line " + std::to_string(ln) + " not recognized: " + L);
        }
    }
    return errors;
}

} // namespace bg
