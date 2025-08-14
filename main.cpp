#include "compiler/banglish.h"
using namespace std;

static void writeTable(const vector<Token>& toks, const SymbolTable& sym){
    // tokens table
    {
        ofstream f("output_tokens.txt");
        for(const auto& t: toks){
            if(t.type=="EOF") continue;
            f << t.lexeme << "\n";
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
        // dynamic widths with larger minimums
        size_t nameW = max<size_t>(12, string("Name").size());
        size_t typeW = max<size_t>(14, string("Type").size());
        size_t lineW = max<size_t>(8, string("Line").size());
        size_t initW = max<size_t>(8, string("Init").size());
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

    // Transpile
    Transpiler tr; tr.toks = lx.tokens; string cpp = tr.transpile(source);

    // Emit tokens and symbol table now (symbol table from transpilation)
    writeTable(lx.tokens, tr.sym);

    // Write transpiled code
    filesystem::create_directory(".generated");
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
