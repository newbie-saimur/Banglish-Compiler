#include "compiler/std.h"
#include "compiler/banglish.h"
#include "compiler/validator.h"
#include "compiler/parser.h"
using namespace std;

static void writeTable(const vector<Token>& toks, const SymbolTable& sym){
    // tokens table (4-column bordered table like the symbol table)
    {
        ofstream f("output_tokens.txt");
    // collect distinct lexemes excluding EOF
    std::set<string> uniq;
    for(const auto& t: toks){ if(t.type!="EOF") uniq.insert(t.lexeme); }
    vector<string> lex(uniq.begin(), uniq.end());

    const int COLS = 3;
        // compute number of rows
        size_t rows = (lex.size() + COLS - 1) / COLS;
        // arrange into grid row-major
    vector<vector<string>> grid(rows, vector<string>(COLS, ""));
        for(size_t i=0;i<lex.size();++i){ grid[i/ COLS][i % COLS] = lex[i]; }
    // column widths: keep overall width equal to previous 4-column (w=28) layout
    vector<size_t> w(COLS, 0);
    auto maxsz=[&](size_t a, size_t b){ return a>b? a:b; };
    const size_t TARGET_INNER = 123; // 4*(28+2) + 3
    const size_t targetSum = TARGET_INNER - (COLS - 1); // sum of (w+2)
    size_t baseEach = (targetSum / COLS);
    if (baseEach >= 2) baseEach -= 2; else baseEach = 0; // convert to w
    size_t rem = targetSum - COLS * (baseEach + 2);
    for(int c=0;c<COLS;++c){ w[c] = baseEach + (c < (int)rem ? 1 : 0); }
        for(size_t r=0;r<rows;++r){ for(int c=0;c<COLS;++c){ w[c] = maxsz(w[c], grid[r][c].size()); } }
        // helpers
        auto border = [&](ostream& o){
            o << '+'; for(int c=0;c<COLS;++c){ o << string(w[c]+2,'-') << '+'; } o << "\n"; };
        auto cell = [&](const string& s, size_t wi){ f << ' ' << left << setw((int)wi) << s << ' '; };
        // render table
        border(f);
            // merged header 'Tokens'
            size_t totalContent = 0; for(int c=0;c<COLS;++c) totalContent += (w[c] + 2);
            // include internal column separators (|) to align with border width
            totalContent += (COLS - 1);
                const string title = "Tokens";
                size_t padL = (totalContent > title.size()) ? (totalContent - title.size())/2 : 0;
                size_t padR = (totalContent > title.size()) ? (totalContent - title.size() - padL) : 0;
                f << '|'
                    << string(padL, ' ') << title << string(padR, ' ')
                    << '|'
                    << "\n";
                border(f);
        for(size_t r=0;r<rows;++r){
            f << '|'; for(int c=0;c<COLS;++c){ cell(grid[r][c], w[c]); f << '|'; } f << "\n";
        }
                border(f);
                // footer: merged, centered like the header
                {
                        size_t totalContent = 0; for(int c=0;c<COLS;++c) totalContent += (w[c] + 2);
                        totalContent += (COLS - 1);
                        std::ostringstream oss; oss << "Total tokens found : " << lex.size();
                        string ft = oss.str();
                        size_t padL = (totalContent > ft.size()) ? (totalContent - ft.size())/2 : 0;
                        size_t padR = (totalContent > ft.size()) ? (totalContent - ft.size() - padL) : 0;
                        f << '|'
                            << string(padL, ' ') << ft << string(padR, ' ')
                            << '|' << "\n";
                        border(f);
                }
    }
    // symbol table
    {
        ofstream f("output_symbol_table.txt");
        vector<Symbol> rows = sym.all();
        auto dispType = [](const string& t)->string{
            if(t == "std::string") return "string";
            // strip leading "std::" if present on any type
            const string p = "std::";
            if(t.rfind(p,0)==0) return t.substr(p.size());
            return t;
        };
    // dynamic widths with larger minimums for extra spacing
    size_t nameW = max<size_t>(22, string("Name").size());
    size_t typeW = max<size_t>(22, string("Type").size());
    size_t lineW = max<size_t>(18, string("Line").size());
    size_t initW = max<size_t>(18, string("Init").size());
        for(const auto& s: rows){
            nameW = max(nameW, s.name.size());
            typeW = max(typeW, dispType(s.dtype).size());
            lineW = max(lineW, to_string(s.line).size());
            initW = max(initW, string(s.initialized?"yes":"no").size());
        }
        auto border = [&](ostream& o){
            o << '+' << string(nameW+2,'-')
              << '+' << string(typeW+2,'-')
              << '+' << string(lineW+2,'-')
              << '+' << string(initW+2,'-')
              << "+\n";
        };
        auto cell = [&](const string& s, size_t w){ f << ' ' << left << setw((int)w) << s << ' '; };

        border(f);
        f << '|' ; cell("Name", nameW); f << '|'; cell("Type", typeW); f << '|'; cell("Line", lineW); f << '|'; cell("Init", initW); f << "|\n";
        border(f);
        for(const auto& s: rows){
            f << '|'; cell(s.name, nameW);
            f << '|'; cell(dispType(s.dtype), typeW);
            f << '|'; cell(to_string(s.line), lineW);
            f << '|'; cell(string(s.initialized?"yes":"no"), initW);
            f << "|\n";
        }
        border(f);
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read Banglish source
    ifstream sf("main.banglish");
    if(!sf){ cerr << "main.banglish not found\n"; return 1; }
    string source((istreambuf_iterator<char>(sf)), istreambuf_iterator<char>());

    // Lex
    Lexer lx(source); lx.lex();

    // Parse and validate with error logging
    ErrorLogger errorLogger("error_log.txt");
    BanglishParser parser(lx.tokens, errorLogger);
    parser.parse();
    
    // Write error log
    errorLogger.writeLog();
    
    // Display error summary
    if (errorLogger.hasErrors()) {
        cerr << "Compilation failed with " << errorLogger.getErrorCount() << " error(s)";
        if (errorLogger.hasWarnings()) {
            cerr << " and " << errorLogger.getWarningCount() << " warning(s)";
        }
        cerr << ". See error_log.txt for details.\n";
        // Continue with transpilation even with errors for educational purposes
    } else if (errorLogger.hasWarnings()) {
        cout << "Compilation successful with " << errorLogger.getWarningCount() << " warning(s). See error_log.txt for details.\n";
    } else {
        cout << "Compilation successful with no errors or warnings.\n";
    }

    // Transpile
    Transpiler tr; tr.toks = lx.tokens; string cpp = tr.transpile(source);

    // Validate tokens and lines
    {
        ofstream vf("output_validation.txt");
        auto terrs = bg::validateTokens(lx.tokens);
        auto lerrs = bg::validateLines(source);
        if(terrs.empty() && lerrs.empty()){
            vf << "OK\n";
        } else {
            for(const auto& e: terrs) vf << e << "\n";
            for(const auto& e: lerrs) vf << e << "\n";
        }
    }

    // Emit tokens and symbol table now (symbol table from transpilation)
    writeTable(lx.tokens, tr.sym);

    // Write transpiled code
    system("mkdir .generated 2>nul || echo Directory exists");
    string transpiledPath = string(".generated/") + "transpiled.cpp";
    ofstream tc(transpiledPath); tc << cpp; tc.close();

    // Compile transpiled code
    string exe;
    string cmd;
#ifdef _WIN32
    exe = string(".generated\\") + "program.exe";
    // Detect available compiler on Windows: prefer cl, else g++
    int hasCL = system("where cl >nul 2>nul");
    if(hasCL == 0){
        cmd = string("cl /nologo /EHsc /std:c++17 \"") + transpiledPath + "\" /Fe:" + exe;
    } else {
        int hasGPP = system("where g++ >nul 2>nul");
        if(hasGPP != 0){
            cerr << "No C++ compiler found (cl or g++). Install Visual Studio Build Tools or MinGW.\n";
            return 2;
        }
        cmd = string("g++ -std=c++17 -O2 -o \"") + exe + "\" \"" + transpiledPath + "\"";
    }
#else
    exe = string(".generated/") + "program";
    cmd = string("g++ -std=c++17 -O2 -o \"") + exe + "\" \"" + transpiledPath + "\"";
#endif

    int ret = system(cmd.c_str());
    if(ret!=0){ cerr << "Compilation of transpiled code failed\n"; return 2; }

    // Run program with input.txt redirected, capture output to output.txt
#ifdef _WIN32
    string runCmd = string("\"") + exe + "\" < input.txt > output.txt";
#else
    string runCmd = string("./") + exe + " < input.txt > output.txt";
#endif
    ret = system(runCmd.c_str());
    if(ret!=0){ cerr << "Program exited with code " << ret << "\n"; }

    return 0;
}
