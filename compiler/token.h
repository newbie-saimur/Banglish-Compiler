#pragma once
#include <string>

struct Token {
    std::string type; // IDENT, KEYWORD, NUMBER, STRING, OP, PUNC, BOOL, CHAR
    std::string lexeme;
    int line;
    int col;
};
