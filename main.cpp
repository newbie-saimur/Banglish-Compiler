// Banglish Compiler Driver: reads Banglish, analyzes, transpiles to C++, compiles, runs
#include "compiler/std.h"
#include "compiler/banglish.h"
#include "compiler/validator.h"
#include "compiler/parser.h"
using namespace std;

// Writes a simple table of unique token lexemes to output_tokens.txt
void writeTokenTable(const vector<Token>& tokens) {
    ofstream file("output_tokens.txt");
    
    set<string> uniqueTokens;
    for(const auto& token : tokens) {
        if(token.type != "EOF") {
            uniqueTokens.insert(token.lexeme);
        }
    }
    
    const int COLS = 3;
    vector<string> tokenList(uniqueTokens.begin(), uniqueTokens.end());
    size_t rows = (tokenList.size() + COLS - 1) / COLS;
    
    // Create grid
    vector<vector<string>> grid(rows, vector<string>(COLS, ""));
    for(size_t i = 0; i < tokenList.size(); ++i) {
        grid[i / COLS][i % COLS] = tokenList[i];
    }
    
    // Calculate column widths
    vector<size_t> colWidths(COLS, 15); // Minimum width
    for(size_t r = 0; r < rows; ++r) {
        for(int c = 0; c < COLS; ++c) {
            colWidths[c] = max(colWidths[c], grid[r][c].size());
        }
    }
    
    // Helper functions
    auto printBorder = [&]() {
        file << '+';
        for(int c = 0; c < COLS; ++c) {
            file << string(colWidths[c] + 2, '-') << '+';
        }
        file << "\n";
    };
    
    auto printCell = [&](const string& text, size_t width) {
        file << ' ' << left << setw(width) << text << ' ';
    };
    
    // Print table
    printBorder();
    
    // Header
    size_t totalWidth = 0;
    for(int c = 0; c < COLS; ++c) {
        totalWidth += colWidths[c] + 2;
    }
    totalWidth += COLS - 1;
    
    string title = "Tokens";
    size_t padding = (totalWidth - title.size()) / 2;
    file << '|' << string(padding, ' ') << title 
         << string(totalWidth - title.size() - padding, ' ') << "|\n";
    
    printBorder();
    
    // Data rows
    for(size_t r = 0; r < rows; ++r) {
        file << '|';
        for(int c = 0; c < COLS; ++c) {
            printCell(grid[r][c], colWidths[c]);
            file << '|';
        }
        file << "\n";
    }
    
    printBorder();
    
    // Footer
    string footer = "Total tokens found : " + to_string(tokenList.size());
    padding = (totalWidth - footer.size()) / 2;
    file << '|' << string(padding, ' ') << footer 
         << string(totalWidth - footer.size() - padding, ' ') << "|\n";
    
    printBorder();
}

// Writes symbol table (name, type, line, initialized) to output_symbol_table.txt
void writeSymbolTable(const SymbolTable& symbolTable) {
    ofstream file("output_symbol_table.txt");
    vector<Symbol> symbols = symbolTable.all();
    
    // Column widths
    size_t nameWidth = max(size_t(15), string("Name").size());
    size_t typeWidth = max(size_t(15), string("Type").size());
    size_t lineWidth = max(size_t(8), string("Line").size());
    size_t initWidth = max(size_t(8), string("Init").size());
    
    // Calculate actual required widths
    for(const auto& symbol : symbols) {
        nameWidth = max(nameWidth, symbol.name.size());
        typeWidth = max(typeWidth, symbol.dtype.size());
        lineWidth = max(lineWidth, to_string(symbol.line).size());
        initWidth = max(initWidth, size_t(3)); // "yes" or "no"
    }
    
    auto printBorder = [&]() {
        file << '+' << string(nameWidth + 2, '-')
             << '+' << string(typeWidth + 2, '-')
             << '+' << string(lineWidth + 2, '-')
             << '+' << string(initWidth + 2, '-') << "+\n";
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
    file << "|\n";
    printBorder();
    
    for(const auto& symbol : symbols) {
        file << '|'; printCell(symbol.name, nameWidth);
        file << '|'; printCell(symbol.dtype, typeWidth);
        file << '|'; printCell(to_string(symbol.line), lineWidth);
        file << '|'; printCell(symbol.initialized ? "yes" : "no", initWidth);
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
