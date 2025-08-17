#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <regex>
#include "token.h"
#include "lexer.h"
struct ParseError {
    int line;
    int col;
    std::string type;
    std::string message;
    std::string context;
    ParseError(int l, int c, const std::string& t, const std::string& m, const std::string& ctx = "")
        : line(l), col(c), type(t), message(m), context(ctx) {}
};
class ErrorLogger {
private:
    std::vector<ParseError> errors;
    std::vector<ParseError> warnings;
    std::string logFile;
public:
    ErrorLogger(const std::string& filename = "error_log.txt") : logFile(filename) {}
    void addError(int line, int col, const std::string& type, const std::string& message, const std::string& context = "") {
        errors.push_back(ParseError(line, col, type, message, context));
    }
    void addWarning(int line, int col, const std::string& type, const std::string& message, const std::string& context = "") {
        warnings.push_back(ParseError(line, col, type, message, context));
    }
    void writeLog() {
        std::ofstream log(logFile);
        log << "=== BANGLISH COMPILER ERROR LOG ===\n\n";
        if (errors.empty() && warnings.empty()) {
            log << "No errors or improvements found.\n";
            return;
        }
        if (!errors.empty()) {
            log << "ERRORS (" << errors.size() << "):\n";
            log << "==================\n";
            for (const auto& error : errors) {
                log << "ERROR [Line " << error.line << ", Col " << error.col << "] "
                    << error.type << ": " << error.message;
                if (!error.context.empty()) {
                    log << "\n  Context: " << error.context;
                }
                log << "\n\n";
            }
        }
        if (!warnings.empty()) {
            log << "IMPROVEMENTS (" << warnings.size() << "):\n";
            log << "====================\n";
            for (const auto& warning : warnings) {
                log << "IMPROVEMENT [Line " << warning.line << ", Col " << warning.col << "] "
                    << warning.type << ": " << warning.message;
                if (!warning.context.empty()) {
                    log << "\n  Context: " << warning.context;
                }
                log << "\n\n";
            }
        }
        log << "=== END OF LOG ===\n";
        log.close();
    }
    bool hasErrors() const { return !errors.empty(); }
    bool hasWarnings() const { return !warnings.empty(); }
    size_t getErrorCount() const { return errors.size(); }
    size_t getWarningCount() const { return warnings.size(); }
};
class BanglishParser {
private:
    std::vector<Token> tokens;
    size_t currentIndex;
    ErrorLogger& logger;
    std::unordered_set<std::string> validKeywords = {
        "shuru", "shesh", "purno sonkha", "dosomik sonkha", "lekha", 
        "akkhor", "sotto-mittha", "jodi", "nahoy jodi", "nahoy", 
        "poro", "dekhao", "loop", "ferot dao"
    };
    std::unordered_set<std::string> validOperators = {
        "+", "-", "*", "/", "%", "++", "--", "+=", "-=", "*=", "/=", "%=",
        "==", "!=", "<=", ">=", "<", ">", "!", "&&", "||", "&", "|",
        "=", "(", ")", "{", "}", "[", "]", ";", ",", "'"
    };
    std::regex identifierPattern{R"(^[A-Za-z_][A-Za-z0-9_]*$)"};
    std::regex numberPattern{R"(^[0-9]+(?:\.[0-9]+)?$)"};
    std::regex stringPattern{R"(^"(?:[^"\\]|\\.)*"$)"};
    std::regex charPattern{R"(^'(?:[^'\\]|\\.)+'$)"};
    std::regex banglishNamingPattern{R"(^[A-Za-z_][A-Za-z0-9_]*$)"};
    std::regex invalidStartPattern{R"(^[0-9].*$)"};
    std::regex invalidCharPattern{R"(.*[\s\-].*)"};
public:
    BanglishParser(const std::vector<Token>& toks, ErrorLogger& log) 
        : tokens(toks), currentIndex(0), logger(log) {}
    Token& current() {
        if (currentIndex >= tokens.size()) {
            static Token eofToken{"EOF", "", 0, 0};
            return eofToken;
        }
        return tokens[currentIndex];
    }
    Token& peek(int offset = 1) {
        size_t index = currentIndex + offset;
        if (index >= tokens.size()) {
            static Token eofToken{"EOF", "", 0, 0};
            return eofToken;
        }
        return tokens[index];
    }
    void advance() {
        if (currentIndex < tokens.size()) {
            currentIndex++;
        }
    }
    void validateKeyword(const Token& token) {
        if (token.type == "KEYWORD") {
            if (validKeywords.find(token.lexeme) == validKeywords.end()) {
                logger.addError(token.line, token.col, "INVALID_KEYWORD", 
                    "Unknown keyword: '" + token.lexeme + "'", 
                    "Expected one of: shuru, shesh, purno sonkha, dosomik sonkha, etc.");
            }
        }
    }
    void validateIdentifier(const Token& token) {
        if (token.type == "IDENT") {
            if (!std::regex_match(token.lexeme, identifierPattern)) {
                logger.addError(token.line, token.col, "INVALID_IDENTIFIER", 
                    "Invalid identifier: '" + token.lexeme + "'",
                    "Identifiers must start with letter or underscore, followed by letters, digits, or underscores");
                return;
            }
            if (std::regex_match(token.lexeme, invalidStartPattern)) {
                logger.addError(token.line, token.col, "INVALID_IDENTIFIER", 
                    "Identifier cannot start with digit: '" + token.lexeme + "'",
                    "Use letters or underscore to start identifier names");
                return;
            }
            if (std::regex_match(token.lexeme, invalidCharPattern)) {
                logger.addError(token.line, token.col, "INVALID_IDENTIFIER", 
                    "Identifier contains invalid characters: '" + token.lexeme + "'",
                    "Use underscore (_) instead of spaces or hyphens");
                return;
            }
            if (validKeywords.find(token.lexeme) != validKeywords.end()) {
                logger.addError(token.line, token.col, "RESERVED_KEYWORD", 
                    "Cannot use reserved keyword as identifier: '" + token.lexeme + "'",
                    "Choose a different name for your variable");
                return;
            }
            validateNamingConvention(token);
        }
    }
    void validateNamingConvention(const Token& token) {
        if (token.lexeme.length() == 1) {
            logger.addWarning(token.line, token.col, "NAMING_CONVENTION", 
                "Single character variable name: '" + token.lexeme + "'",
                "Consider using more descriptive names");
        }
        if (token.lexeme == "temp" || token.lexeme == "tmp" || token.lexeme == "var") {
            logger.addWarning(token.line, token.col, "NAMING_CONVENTION", 
                "Generic variable name: '" + token.lexeme + "'",
                "Use more specific and meaningful names");
        }
        bool allCaps = true;
        for (char c : token.lexeme) {
            if (islower(c)) {
                allCaps = false;
                break;
            }
        }
        if (allCaps && token.lexeme.length() > 1) {
            logger.addWarning(token.line, token.col, "NAMING_CONVENTION", 
                "All-caps variable name: '" + token.lexeme + "'",
                "Reserve all-caps for constants, use camelCase or snake_case for variables");
        }
    }
    void validateOperator(const Token& token) {
        if (token.type == "OP") {
            if (validOperators.find(token.lexeme) == validOperators.end()) {
                logger.addError(token.line, token.col, "INVALID_OPERATOR", 
                    "Unknown operator: '" + token.lexeme + "'",
                    "Check for typos in operator usage");
            }
        }
    }
    void validateLiteral(const Token& token) {
        if (token.type == "NUMBER") {
            if (!std::regex_match(token.lexeme, numberPattern)) {
                logger.addError(token.line, token.col, "INVALID_NUMBER", 
                    "Invalid number format: '" + token.lexeme + "'",
                    "Numbers should be integers or decimals (e.g., 123, 45.67)");
            }
        } else if (token.type == "STRING") {
            if (!std::regex_match(token.lexeme, stringPattern)) {
                logger.addError(token.line, token.col, "INVALID_STRING", 
                    "Invalid string format: '" + token.lexeme + "'",
                    "Strings should be enclosed in double quotes");
            }
        }
    }
    void validateStructure() {
        bool hasShuru = false;
        bool hasShesh = false;
        int braceDepth = 0;
        int parenDepth = 0;
        int bracketDepth = 0;
        for (const auto& token : tokens) {
            if (token.type == "KEYWORD") {
                if (token.lexeme == "shuru") {
                    if (hasShuru) {
                        logger.addError(token.line, token.col, "DUPLICATE_SHURU", 
                            "Multiple 'shuru' statements found",
                            "Program should have only one 'shuru' at the beginning");
                    }
                    hasShuru = true;
                } else if (token.lexeme == "shesh") {
                    if (hasShesh) {
                        logger.addError(token.line, token.col, "DUPLICATE_SHESH", 
                            "Multiple 'shesh' statements found",
                            "Program should have only one 'shesh' at the end");
                    }
                    hasShesh = true;
                }
            } else if (token.type == "OP") {
                if (token.lexeme == "{") braceDepth++;
                else if (token.lexeme == "}") braceDepth--;
                else if (token.lexeme == "(") parenDepth++;
                else if (token.lexeme == ")") parenDepth--;
                else if (token.lexeme == "[") bracketDepth++;
                else if (token.lexeme == "]") bracketDepth--;
                if (braceDepth < 0) {
                    logger.addError(token.line, token.col, "UNMATCHED_BRACE", 
                        "Closing brace '}' without matching opening brace '{'",
                        "Check brace pairing in your code");
                }
                if (parenDepth < 0) {
                    logger.addError(token.line, token.col, "UNMATCHED_PAREN", 
                        "Closing parenthesis ')' without matching opening parenthesis '('",
                        "Check parenthesis pairing in your code");
                }
                if (bracketDepth < 0) {
                    logger.addError(token.line, token.col, "UNMATCHED_BRACKET", 
                        "Closing bracket ']' without matching opening bracket '['",
                        "Check bracket pairing in your code");
                }
            }
        }
        if (!hasShuru) {
            logger.addError(1, 1, "MISSING_SHURU", 
                "Program must start with 'shuru'",
                "Add 'shuru' at the beginning of your program");
        }
        if (!hasShesh) {
            int lastLine = tokens.empty() ? 1 : tokens.back().line;
            logger.addError(lastLine, 1, "MISSING_SHESH", 
                "Program must end with 'shesh'",
                "Add 'shesh' at the end of your program");
        }
        if (braceDepth > 0) {
            logger.addError(tokens.back().line, tokens.back().col, "UNCLOSED_BRACE", 
                std::to_string(braceDepth) + " unclosed brace(s) '{'",
                "Add missing closing brace(s) '}'");
        }
        if (parenDepth > 0) {
            logger.addError(tokens.back().line, tokens.back().col, "UNCLOSED_PAREN", 
                std::to_string(parenDepth) + " unclosed parenthesis(es) '('",
                "Add missing closing parenthesis(es) ')'");
        }
        if (bracketDepth > 0) {
            logger.addError(tokens.back().line, tokens.back().col, "UNCLOSED_BRACKET", 
                std::to_string(bracketDepth) + " unclosed bracket(s) '['",
                "Add missing closing bracket(s) ']'");
        }
    }
    void parse() {
        for (const auto& token : tokens) {
            if (token.type == "ERROR") {
                if (token.lexeme == "UNCLOSED_STRING") {
                    logger.addError(token.line, token.col, "UNCLOSED_STRING", 
                        "String literal is not properly closed",
                        "Add closing quote (\") to end the string");
                }
                continue;
            }
            validateKeyword(token);
            validateIdentifier(token);
            validateOperator(token);
            validateLiteral(token);
        }
        validateStructure();
        validateStatementSyntax();
    }
    void validateStatementSyntax() {
        currentIndex = 0;
        while (current().type != "EOF") {
            if (current().type == "KEYWORD") {
                validateStatement();
            } else {
                advance();
            }
        }
    }
    void validateStatement() {
        Token& token = current();
        if (token.lexeme == "purno sonkha" || token.lexeme == "dosomik sonkha" || 
            token.lexeme == "lekha" || token.lexeme == "akkhor" || token.lexeme == "sotto-mittha") {
            validateDeclaration();
        } else if (token.lexeme == "jodi") {
            validateIfStatement();
        } else if (token.lexeme == "loop") {
            validateLoopStatement();
        } else if (token.lexeme == "poro") {
            validateInputStatement();
        } else if (token.lexeme == "dekhao") {
            validateOutputStatement();
        } else if (token.lexeme == "ferot dao") {
            validateReturnStatement();
        } else {
            advance();
        }
    }
    void validateDeclaration() {
        Token& typeToken = current();
        advance();
        if (current().type != "IDENT") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected identifier after type declaration",
                "Declaration syntax: " + typeToken.lexeme + " variable_name;");
            return;
        }
        advance();
        if (current().lexeme == "[") {
            advance();
            if (current().type != "NUMBER" && current().type != "IDENT") {
                logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                    "Expected array size (number or variable)",
                    "Array syntax: type variable[size];");
            }
            advance();
            if (current().lexeme != "]") {
                logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                    "Expected ']' after array size",
                    "Array syntax: type variable[size];");
            }
            advance();
        }
        if (current().lexeme == "=") {
            advance();
            if (current().lexeme == ";") {
                logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                    "Expected value after assignment operator",
                    "Assignment syntax: variable = value;");
            }
        }
        while (current().type != "EOF" && current().lexeme != ";") {
            advance();
        }
    }
    void validateIfStatement() {
        advance();
        if (current().lexeme != "(") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected '(' after 'jodi'",
                "If syntax: jodi (condition) { ... }");
            return;
        }
        advance();
        int parenCount = 1;
        while (current().type != "EOF" && parenCount > 0) {
            if (current().lexeme == "(") parenCount++;
            else if (current().lexeme == ")") parenCount--;
            advance();
        }
        if (parenCount > 0) {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Unclosed parenthesis in if condition",
                "If syntax: jodi (condition) { ... }");
        }
    }
    void validateLoopStatement() {
        advance();
        if (current().lexeme != "(") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected '(' after 'loop'",
                "Loop syntax: loop (init; condition; update) { ... }");
            return;
        }
        advance();
        int semicolonCount = 0;
        int parenCount = 1;
        while (current().type != "EOF" && parenCount > 0) {
            if (current().lexeme == "(") parenCount++;
            else if (current().lexeme == ")") parenCount--;
            else if (current().lexeme == ";" && parenCount == 1) semicolonCount++;
            advance();
        }
        if (semicolonCount != 2) {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Loop requires exactly 2 semicolons for 3 parts",
                "Loop syntax: loop (init; condition; update) { ... }");
        }
    }
    void validateInputStatement() {
        advance();
        if (current().lexeme != "(") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected '(' after 'poro'",
                "Input syntax: poro(variable);");
            return;
        }
        advance();
        if (current().type != "IDENT") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected variable name in input statement",
                "Input syntax: poro(variable);");
        } else {
            advance();
            if (current().lexeme == "[") {
                advance();
                int bracketCount = 1;
                while (current().type != "EOF" && bracketCount > 0) {
                    if (current().lexeme == "[") bracketCount++;
                    else if (current().lexeme == "]") bracketCount--;
                    advance();
                }
            }
        }
        if (current().lexeme != ")") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected ')' after variable name",
                "Input syntax: poro(variable);");
        } else {
            advance();
        }
    }
    void validateOutputStatement() {
        advance();
        if (current().type == "EOF" || current().lexeme == ";") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected output expression after 'dekhao'",
                "Output syntax: dekhao \"text\" or dekhao variable;");
        }
        while (current().type != "EOF" && current().lexeme != ";") {
            advance();
        }
    }
    void validateReturnStatement() {
        Token& token = current();
        if (token.lexeme == "ferot dao") {
            advance();
        } else {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Invalid return statement: '" + token.lexeme + "'",
                "Return syntax: ferot dao value;");
            return;
        }
        if (current().type == "EOF" || current().lexeme == ";") {
            logger.addError(current().line, current().col, "SYNTAX_ERROR", 
                "Expected return value after 'ferot dao'",
                "Return syntax: ferot dao value;");
        }
        while (current().type != "EOF" && current().lexeme != ";") {
            advance();
        }
    }
};
