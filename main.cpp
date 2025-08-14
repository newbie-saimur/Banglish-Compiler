// Portable includes (avoid <bits/stdc++.h> for MSVC)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <filesystem>
#include <cctype>
using namespace std;

struct Token {
    string type; // IDENT, KEYWORD, NUMBER, STRING, OP, PUNC, BOOL, CHAR
    string lexeme;
    int line;
    int col;
};

struct Symbol {
    string name;
    string dtype; // int,float,string,char,bool
    int line;
    bool initialized;
};

static const set<string> keywords = {
    "shuru","shesh","purno","sonkha","dosomik","lekha","akkhor","sotto-mittha",
    "jodi","nahoy","poro","dekhao","loop"
};

static const set<string> simpleOps = {
    "+","-","*","/","%","++","--","+=","-=","*=","/=",
    "<=",">=","<",">","!","==","!=","=",
    "&&","||","&","|",
    "(",")","{","}",";","," 
};

static bool isIdentStart(char c){ return isalpha((unsigned char)c) || c=='_'; }
static bool isIdentChar(char c){ return isalnum((unsigned char)c) || c=='_'; }

struct Lexer {
    string src; vector<Token> tokens; int i=0; int line=1; int col=1;
    Lexer(const string&s):src(s){}

    char peek(int k=0){ if(i+k<(int)src.size()) return src[i+k]; return '\0'; }
    char get(){ char c=peek(); if(c=='\n'){ line++; col=1; } else col++; i++; return c; }
    void add(const string&type,const string&lex,int l,int c){ tokens.push_back({type,lex,l,c}); }

    void lex(){
        while(true){
            char c=peek(); if(c=='\0') break;
            if(isspace((unsigned char)c)){ get(); continue; }
            int l=line, c0=col;
            // comments: //...
            if(c=='/' && peek(1)=='/') { while(peek()!='\n' && peek()!='\0') get(); continue; }
            // string literal
            if(c=='"'){
                get(); string val; bool ok=true;
                while(true){ char d=peek(); if(d=='\0') { ok=false; break; }
                    if(d=='\\'){ get(); char e=get(); val.push_back('\\'); val.push_back(e); }
                    else if(d=='"'){ get(); break; }
                    else { val.push_back(get()); }
                }
                add("STRING","\""+val+"\"",l,c0); continue;
            }
            // number
            if(isdigit((unsigned char)c)){
                string num; bool hasDot=false;
                while(isdigit((unsigned char)peek()) || (!hasDot && peek()=='.')){
                    if(peek()=='.') hasDot=true; num.push_back(get());
                }
                add("NUMBER",num,l,c0); continue;
            }
            // identifier or multi-word/compound keyword fragments
            if(isIdentStart(c)){
                string id; id.push_back(get());
                while(isIdentChar(peek())) id.push_back(get());
                // handle possible two-word combos like "purno sonkha", "dosomik sonkha", "ferot dao", "nahoy jodi"
                // lookahead for a space then next ident
                int saveI=i, saveL=line, saveC=col;
                // skip spaces
                int j=i; while(isspace((unsigned char)src[j]) && src[j]!='\n') j++;
                // if next is ident, capture
                if(j<(int)src.size() && (isalpha((unsigned char)src[j])||src[j]=='_')){
                    int k=j; string id2; id2.push_back(src[k++]);
                    while(k<(int)src.size() && (isalnum((unsigned char)src[k])||src[k]=='_')) id2.push_back(src[k++]);
                    string combo = id + string(" ") + id2;
                    if(combo=="purno sonkha" || combo=="dosomik sonkha" || combo=="ferot dao" || combo=="nahoy jodi"){
                        // consume spaces and second ident
                        while(i<k) { get(); }
                        add("KEYWORD",combo,l,c0); continue;
                    }
                }
                // handle hyphenated keyword "sotto-mittha"
                if(peek()== '-'){
                    int j=i+1; // skip '-'
                    if(j<(int)src.size() && (isalpha((unsigned char)src[j])||src[j]=='_')){
                        int k=j; string id2; id2.push_back(src[k++]);
                        while(k<(int)src.size() && (isalnum((unsigned char)src[k])||src[k]=='_')) id2.push_back(src[k++]);
                        string combo = id + string("-") + id2;
                        if(combo=="sotto-mittha"){
                            while(i<k) { get(); }
                            add("KEYWORD",combo,l,c0); continue;
                        }
                    }
                }
                // map Banglish to logical keyword token
                if(keywords.count(id)) add("KEYWORD",id,l,c0);
                else add("IDENT",id,l,c0);
                continue;
            }
            // operators and punctuation: prefer two-char then one-char
            string two; two.push_back(c); two.push_back(peek(1));
            if(simpleOps.count(two)) { get(); get(); add("OP",two,l,c0); continue; }
            string one; one.push_back(c);
            if(simpleOps.count(one)) { get(); add("OP",one,l,c0); continue; }
            // unknown
            get(); add("OP",string(1,c),l,c0);
        }
        add("EOF","",line,col);
    }
};

struct SymbolTable {
    // very simple single-scope table for demo
    unordered_map<string, Symbol> table;
    vector<string> order;
    void declare(const string& name, const string& dtype, int line){
        if(!table.count(name)) order.push_back(name);
        table[name] = {name,dtype,line,false};
    }
    void initialize(const string& name){ if(table.count(name)) table[name].initialized=true; }
    vector<Symbol> all() const{
        vector<Symbol> v; v.reserve(order.size());
        for(auto &n: order){ auto it=table.find(n); if(it!=table.end()) v.push_back(it->second); }
        return v;
    }
};

static string trim(const string&s){ size_t a=s.find_first_not_of(" \t\r\n"); if(a==string::npos) return ""; size_t b=s.find_last_not_of(" \t\r\n"); return s.substr(a,b-a+1);}    

// Very lightweight transpiler using tokens + line-based heuristics
struct Transpiler {
    vector<Token> toks;
    SymbolTable sym;

    static string mapType(const string& kw){
        if(kw=="purno sonkha") return "int";
        if(kw=="dosomik sonkha") return "double";
        if(kw=="lekha") return "std::string";
        if(kw=="akkhor") return "char";
        if(kw=="sotto-mittha") return "bool";
        return "";
    }

    // Render a format string with {var} placeholders to C++ concatenation
    static string renderDekhao(const string& inside){
        // inside is either expression or a quoted string token \"...\"
        string s = trim(inside);
        // special: raw \n triggers newline without quotes
        if(s == "\\n"){
            return "std::cout << '\\n';";
        }
        if(s.size()>=2 && s.front()=='"' && s.back()=='"'){
            string body = s.substr(1, s.size()-2);
            // split by { and }
            string out;
            out += "";
            string cur; bool inBrace=false; vector<string> parts; vector<bool> isExpr;
            for(size_t i=0;i<body.size();++i){
                char c=body[i];
                if(!inBrace && c=='{' ){ inBrace=true; if(!cur.empty()){ parts.push_back(cur); isExpr.push_back(false); cur.clear(); } }
                else if(inBrace && c=='}'){ inBrace=false; if(!cur.empty()){ parts.push_back(cur); isExpr.push_back(true); cur.clear(); } }
                else cur.push_back(c);
            }
            if(!cur.empty()){ parts.push_back(cur); isExpr.push_back(false); }
            // build cout expression
            string expr;
            if(parts.empty()) expr = "\"\"";
        for(size_t i=0;i<parts.size();++i){
                if(i>0) expr += " << ";
                if(isExpr[i]) expr += "(" + parts[i] + ")";
                else {
            // keep C-style escapes (\n, \t, \", \\) as-is
            expr += "\"" + parts[i] + "\"";
                }
            }
            return "std::cout << " + expr + ";";
        } else {
            return "std::cout << (" + s + ");";
        }
    }

    static bool isTypeKeyword(const string& k){
        return k=="purno sonkha"||k=="dosomik sonkha"||k=="lekha"||k=="akkhor"||k=="sotto-mittha";
    }

    // A very simple parser that walks the original source lines and emits C++
    string transpile(const string& source){
        stringstream in(source); string line; vector<string> out;
    out.push_back("#include <iostream>");
    out.push_back("#include <string>");
    out.push_back("#include <vector>");
    out.push_back("#include <sstream>");
    out.push_back("#include <iomanip>");
    out.push_back("#include <unordered_map>");
    out.push_back("#include <set>");
    out.push_back("using namespace std;");
        out.push_back("int main(){");

        int lineNo=0;
        while(getline(in,line)){
            lineNo++;
            string L = trim(line);
            if(L.empty()) continue;
            if(L=="shuru" || L=="shesh") continue;
            // If line begins with one or more closing braces, emit them and parse the remainder
            while(!L.empty() && L[0]=='}'){
                out.push_back("}");
                L = trim(L.substr(1));
            }
            if(L.empty()) continue;

            // Replace multi-word keywords in context for easier handling
            // 1. Declarations: <type> name [= expr];
            auto handleDecl = [&](const string& tkw){
                string cxxType = mapType(tkw);
                string rest = trim(L.substr(tkw.size()));
                if(!rest.empty() && rest[0]==' ') rest = trim(rest);
                // split on '=' if present
                string name, init;
                size_t eq = rest.find('=');
                if(eq==string::npos){ name = trim(rest); }
                else{ name = trim(rest.substr(0,eq)); init = trim(rest.substr(eq+1)); }
                if(!name.empty() && name.back()==';') name.pop_back();
                if(!init.empty() && init.back()==';') init.pop_back();
                if(!name.empty()) sym.declare(name,cxxType,lineNo);
                if(!init.empty()) sym.initialize(name);
                string stmt = cxxType + " " + name;
                if(!init.empty()) stmt += " = " + init;
                stmt += ";";
                out.push_back(stmt);
                return true;
            };

            if(L.rfind("purno sonkha",0)==0){ if(handleDecl("purno sonkha")) continue; }
            if(L.rfind("dosomik sonkha",0)==0){ if(handleDecl("dosomik sonkha")) continue; }
            if(L.rfind("lekha",0)==0){ if(handleDecl("lekha")) continue; }
            if(L.rfind("akkhor",0)==0){ if(handleDecl("akkhor")) continue; }
            if(L.rfind("sotto-mittha",0)==0){ if(handleDecl("sotto-mittha")) continue; }

            // Input: poro (x);
            if(L.rfind("poro",0)==0){
                // poro (x)
                size_t lp = L.find('('), rp=L.find(')');
                if(lp!=string::npos && rp!=string::npos && rp>lp){
                    string var = trim(L.substr(lp+1, rp-lp-1));
                    if(!var.empty() && var.back()==';') var.pop_back();
                    sym.initialize(var);
                    // choose extraction based on previously declared type
                    string type=""; if(sym.table.count(var)) type = sym.table[var].dtype;
                    string stmt;
                    if(type=="std::string") stmt = "getline(cin >> ws, "+var+");";
                    else stmt = "cin >> "+var+";";
                    out.push_back(stmt);
                    continue;
                }
            }

            // Print: dekhao ...;
            if(L.rfind("dekhao",0)==0){
                string inside = trim(L.substr(string("dekhao").size()));
                if(!inside.empty() && inside[0]==' ') inside = trim(inside);
                if(!inside.empty() && inside.back()==';') inside.pop_back();
                out.push_back(renderDekhao(inside));
                continue;
            }

            // If/else if/else
            if(L.rfind("jodi",0)==0){
                // jodi (cond) {
                string rest = trim(L.substr(4));
                out.push_back("if " + rest);
                continue;
            }
            if(L.rfind("nahoy jodi",0)==0){
                string rest = trim(L.substr(string("nahoy jodi").size()));
                out.push_back("else if " + rest);
                continue;
            }
            if(L.rfind("nahoy",0)==0 && L.rfind("nahoy jodi",0)!=0){
                string rest = trim(L.substr(string("nahoy").size()));
                if(rest.empty()) out.push_back("else");
                else out.push_back(string("else ") + rest);
                continue;
            }

            // Return: ferot dao expr;
            if(L.rfind("ferot dao",0)==0){
                string expr = trim(L.substr(string("ferot dao").size()));
                if(!expr.empty() && expr.back()==';') expr.pop_back();
                out.push_back("return " + expr + ";");
                continue;
            }

            // Loop: Banglish 'loop' -> C++ 'for'
            if(L.rfind("loop",0)==0){ 
                string X=L; 
                // replace leading 'loop' with 'for'
                X = string("for") + L.substr(4);
                // replace Banglish types with C++ types in the line
                auto rep=[&](const string& a,const string& b){
                    size_t pos=0; while((pos=X.find(a,pos))!=string::npos){ X.replace(pos,a.size(),b); pos+=b.size(); }
                };
                rep("purno sonkha","int");
                rep("dosomik sonkha","double");
                rep("lekha","std::string");
                rep("akkhor","char");
                rep("sotto-mittha","bool");
                // Try to record loop var declaration in symbol table
                size_t lp = L.find('('), sc = L.find(';', lp==string::npos?0:lp);
                if(lp!=string::npos && sc!=string::npos && sc>lp){
                    string init = trim(L.substr(lp+1, sc-lp-1));
                    // expect like: "purno sonkha i = 0" or "int i = 0" after replacement
                    vector<pair<string,string>> types = {
                        {"purno sonkha","int"},{"dosomik sonkha","double"},{"lekha","std::string"},{"akkhor","char"},{"sotto-mittha","bool"},
                        {"int","int"},{"double","double"},{"std::string","std::string"},{"char","char"},{"bool","bool"}
                    };
                    for(auto &p: types){
                        const string& t=p.first; const string& cxx=p.second;
                        if(init.rfind(t,0)==0){
                            string rest = trim(init.substr(t.size()));
                            if(!rest.empty() && rest[0]==' ') rest = trim(rest);
                            string name = rest;
                            size_t eq = rest.find('='); if(eq!=string::npos) name = trim(rest.substr(0,eq));
                            if(!name.empty()) sym.declare(name,cxx,lineNo), sym.initialize(name);
                            break;
                        }
                    }
                }
                out.push_back(X); continue; 
            }

            // Otherwise, pass-through (assignments, braces, etc.)
            // Attempt to detect simple assignment: name = expr;
            {
                size_t eq = L.find('=');
                if(eq!=string::npos){
                    string lhs = trim(L.substr(0,eq));
                    // drop possible trailing type on lhs already handled earlier, so here just name
                    // remove potential spaces before semicolon in the whole line
                    size_t sc = L.find(';', eq);
                    if(!lhs.empty() && isalpha((unsigned char)lhs[0])){
                        sym.initialize(lhs);
                    }
                }
            }
            out.push_back(L);
        }

        out.push_back("}");
        // join
        string code; for(auto&s:out){ code += s + "\n"; }
        return code;
    }
};

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
