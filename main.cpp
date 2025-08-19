// Banglish Compiler Driver: reads Banglish, analyzes, transpiles to C++, compiles, runs
#include "compiler/std.h"
#include "compiler/banglish.h"
#include "compiler/validator.h"
#include "compiler/parser.h"
using namespace std;

// Writes a table of unique tokens with types and lexemes in 4 columns to output_tokens.txt
void writeTokenTable(const vector<Token>& tokens) {
    ofstream file("output_tokens.txt");
    
    // Collect unique tokens using a map to preserve type-lexeme pairs
    map<pair<string, string>, bool> uniqueTokenMap;
    for(const auto& token : tokens) {
        if(token.type != "EOF") {
            uniqueTokenMap[{token.type, token.lexeme}] = true;
        }
    }
    
    // Convert to vector for easier processing
    vector<pair<string, string>> uniqueTokens;
    for(const auto& entry : uniqueTokenMap) {
        uniqueTokens.push_back(entry.first);
    }
    
    // Arrange in 4 columns: Token Type | Lexeme | Token Type | Lexeme
    const int COLS = 2; // 2 pairs of (Token Type, Lexeme)
    size_t rows = (uniqueTokens.size() + COLS - 1) / COLS;
    
    // Column widths
    size_t typeWidth1 = max(size_t(12), string("Token Type").size());
    size_t lexemeWidth1 = max(size_t(15), string("Lexeme").size());
    size_t typeWidth2 = max(size_t(12), string("Token Type").size());
    size_t lexemeWidth2 = max(size_t(15), string("Lexeme").size());
    
    // Calculate actual required widths for both columns
    for(size_t i = 0; i < uniqueTokens.size(); ++i) {
        if(i % 2 == 0) { // Left side (first pair)
            typeWidth1 = max(typeWidth1, uniqueTokens[i].first.size());
            lexemeWidth1 = max(lexemeWidth1, uniqueTokens[i].second.size());
        } else { // Right side (second pair)
            typeWidth2 = max(typeWidth2, uniqueTokens[i].first.size());
            lexemeWidth2 = max(lexemeWidth2, uniqueTokens[i].second.size());
        }
    }
    
    // Helper functions
    auto printBorder = [&]() {
        file << '+' << string(typeWidth1 + 2, '-')
             << '+' << string(lexemeWidth1 + 2, '-')
             << '+' << string(typeWidth2 + 2, '-')
             << '+' << string(lexemeWidth2 + 2, '-') << "+\n";
    };
    
    auto printCell = [&](const string& text, size_t width) {
        file << ' ' << left << setw(width) << text << ' ';
    };
    
    // Print table
    printBorder();
    
    // Header
    file << '|'; printCell("Token Type", typeWidth1);
    file << '|'; printCell("Lexeme", lexemeWidth1);
    file << '|'; printCell("Token Type", typeWidth2);
    file << '|'; printCell("Lexeme", lexemeWidth2);
    file << "|\n";
    printBorder();
    
    // Data rows
    for(size_t row = 0; row < rows; ++row) {
        file << '|';
        
        // Left pair (Token Type | Lexeme)
        size_t leftIndex = row * 2;
        if(leftIndex < uniqueTokens.size()) {
            printCell(uniqueTokens[leftIndex].first, typeWidth1);
            file << '|';
            printCell(uniqueTokens[leftIndex].second, lexemeWidth1);
        } else {
            printCell("", typeWidth1);
            file << '|';
            printCell("", lexemeWidth1);
        }
        
        file << '|';
        
        // Right pair (Token Type | Lexeme)
        size_t rightIndex = row * 2 + 1;
        if(rightIndex < uniqueTokens.size()) {
            printCell(uniqueTokens[rightIndex].first, typeWidth2);
            file << '|';
            printCell(uniqueTokens[rightIndex].second, lexemeWidth2);
        } else {
            printCell("", typeWidth2);
            file << '|';
            printCell("", lexemeWidth2);
        }
        
        file << "|\n";
    }
    
    printBorder();
    
    // Footer
    size_t totalWidth = typeWidth1 + lexemeWidth1 + typeWidth2 + lexemeWidth2 + 10; // +10 for borders and spaces
    string footer = "Unique tokens: " + to_string(uniqueTokens.size());
    size_t padding = (totalWidth - footer.size()) / 2;
    file << '|' << string(padding, ' ') << footer 
         << string(totalWidth - footer.size() - padding, ' ') << "|\n";
    
    printBorder();
}

// Writes symbol table (name, type, line, initialized, value) to output_symbol_table.txt
void writeSymbolTable(const SymbolTable& symbolTable) {
    ofstream file("output_symbol_table.txt");
    vector<Symbol> symbols = symbolTable.all();
    
    // Column widths
    size_t nameWidth = max(size_t(15), string("Name").size());
    size_t typeWidth = max(size_t(15), string("Type").size());
    size_t lineWidth = max(size_t(8), string("Line").size());
    size_t initWidth = max(size_t(8), string("Init").size());
    size_t valueWidth = max(size_t(15), string("Value").size());
    
    // Calculate actual required widths
    for(const auto& symbol : symbols) {
        nameWidth = max(nameWidth, symbol.name.size());
        typeWidth = max(typeWidth, symbol.dtype.size());
        lineWidth = max(lineWidth, to_string(symbol.line).size());
        initWidth = max(initWidth, size_t(3)); // "yes" or "no"
        valueWidth = max(valueWidth, symbol.value.empty() ? size_t(5) : symbol.value.size()); // "N/A" or actual value
    }
    
    auto printBorder = [&]() {
        file << '+' << string(nameWidth + 2, '-')
             << '+' << string(typeWidth + 2, '-')
             << '+' << string(lineWidth + 2, '-')
             << '+' << string(initWidth + 2, '-')
             << '+' << string(valueWidth + 2, '-') << "+\n";
    };
    
    auto printCell = [&](const string& text, size_t width) {
        file << ' ' << left << setw(width) << text << ' ';
    };
    
    // Print table
    printBorder();
    file << '|'; printCell("Name", nameWidth);
    file << '|'; printCell("Type", typeWidth);
    file << '|'; printCell("Line", lineWidth);
    file << '|'; printCell("Init", initWidth);
    file << '|'; printCell("Value", valueWidth);
    file << "|\n";
    printBorder();
    
    for(const auto& symbol : symbols) {
        file << '|'; printCell(symbol.name, nameWidth);
        file << '|'; printCell(symbol.dtype, typeWidth);
        file << '|'; printCell(to_string(symbol.line), lineWidth);
        file << '|'; printCell(symbol.initialized ? "yes" : "no", initWidth);
        file << '|'; printCell(symbol.value.empty() ? "N/A" : symbol.value, valueWidth);
        file << "|\n";
    }
    
    printBorder();
}

// Reads the entire source file into a string
string readSourceFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << "\n";
        exit(1);
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

// Validates tokens and lines, writes OK or issues to output_validation.txt
void writeValidation(const vector<Token>& tokens, const string& source) {
    ofstream file("output_validation.txt");
    auto tokenErrors = bg::validateTokens(tokens);
    auto lineErrors = bg::validateLines(source);
    
    if (tokenErrors.empty() && lineErrors.empty()) {
        file << "OK\n";
    } else {
        for (const auto& error : tokenErrors) {
            file << error << "\n";
        }
        for (const auto& error : lineErrors) {
            file << error << "\n";
        }
    }
}

// Chooses a compiler command (cl or g++) for the current platform
string getCompilerCommand(const string& sourceFile, const string& outputFile) {
#ifdef _WIN32
    if (system("where cl >nul 2>nul") == 0) {
        return "cl /nologo /EHsc /std:c++17 \"" + sourceFile + "\" /Fe:" + outputFile;
    } else if (system("where g++ >nul 2>nul") == 0) {
        return "g++ -std=c++17 -O2 -o \"" + outputFile + "\" \"" + sourceFile + "\"";
    } else {
        cerr << "Error: No C++ compiler found (cl or g++)\n";
        exit(2);
    }
#else
    return "g++ -std=c++17 -O2 -o \"" + outputFile + "\" \"" + sourceFile + "\"";
#endif
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Read Banglish source
    string source = readSourceFile("main.banglish");
    
    // Lex: tokenize source
    Lexer lexer(source);
    lexer.lex();
    
    // Parse + validate (writes error_log.txt)
    ErrorLogger errorLogger("error_log.txt");
    BanglishParser parser(lexer.tokens, errorLogger);
    parser.parse();
    errorLogger.writeLog();
    
    // Report status to console
    if (errorLogger.hasErrors()) {
        cerr << "Compilation failed with " << errorLogger.getErrorCount() << " error(s)";
        if (errorLogger.hasWarnings()) {
            cerr << " and " << errorLogger.getWarningCount() << " improvement(s)";
        }
        cerr << ". See error_log.txt for details.\n";
    } else if (errorLogger.hasWarnings()) {
        cout << "Compilation successful with " << errorLogger.getWarningCount() 
             << " improvement(s). See error_log.txt for details.\n";
    } else {
        cout << "Compilation successful with no errors or improvements.\n";
    }
    
    // Transpile Banglish -> C++
    Transpiler transpiler;
    transpiler.toks = lexer.tokens;
    string cppCode = transpiler.transpile(source);
    
    // Write validation, tokens, symbols
    writeValidation(lexer.tokens, source);
    writeTokenTable(lexer.tokens);
    writeSymbolTable(transpiler.sym);
    
    // Ensure .generated exists
    system("mkdir .generated 2>nul || echo Directory exists");
    
    // Emit transpiled.cpp and compile to program(.exe)
    string transpiledPath = ".generated/transpiled.cpp";
    ofstream transpiledFile(transpiledPath);
    transpiledFile << cppCode;
    transpiledFile.close();
    
    string executablePath;
#ifdef _WIN32
    executablePath = ".generated\\program.exe";
#else
    executablePath = ".generated/program";
#endif
    
    string compileCommand = getCompilerCommand(transpiledPath, executablePath);
    if (system(compileCommand.c_str()) != 0) {
        cerr << "Error: Compilation of transpiled code failed\n";
        return 2;
    }
    
    // Run compiled program with input.txt -> output.txt
#ifdef _WIN32
    string runCommand = "\"" + executablePath + "\" < input.txt > output.txt";
#else
    string runCommand = "./" + executablePath + " < input.txt > output.txt";
#endif
    
    int exitCode = system(runCommand.c_str());
    if (exitCode != 0) {
        cerr << "Program exited with code " << exitCode << "\n";
    }
    
    // Done
    return 0;
}
