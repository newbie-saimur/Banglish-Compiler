#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "token.h"
#include "symbol_table.h"

static std::string trim_str(const std::string&s){ size_t a=s.find_first_not_of(" \t\r\n"); if(a==std::string::npos) return ""; size_t b=s.find_last_not_of(" \t\r\n"); return s.substr(a,b-a+1);}    

struct Transpiler {
    std::vector<Token> toks;
    SymbolTable sym;

    static std::string mapType(const std::string& kw){
        if(kw=="purno sonkha") return "int";
        if(kw=="dosomik sonkha") return "double";
        if(kw=="lekha") return "std::string";
        if(kw=="akkhor") return "char";
        if(kw=="sotto-mittha") return "bool";
        return "";
    }

    static std::string renderDekhao(const std::string& inside){
        std::string s = trim_str(inside);
        if(s == "\\n"){
            return "std::cout << '\\n';";
        }
        if(s.size()>=2 && s.front()=='"' && s.back()=='"'){
            std::string body = s.substr(1, s.size()-2);
            std::string cur; bool inBrace=false; std::vector<std::string> parts; std::vector<bool> isExpr;
            for(size_t i=0;i<body.size();++i){
                char c=body[i];
                if(!inBrace && c=='{'){ inBrace=true; if(!cur.empty()){ parts.push_back(cur); isExpr.push_back(false); cur.clear(); } }
                else if(inBrace && c=='}'){ inBrace=false; if(!cur.empty()){ parts.push_back(cur); isExpr.push_back(true); cur.clear(); } }
                else cur.push_back(c);
            }
            if(!cur.empty()){ parts.push_back(cur); isExpr.push_back(false); }
            std::string expr; if(parts.empty()) expr = "\"\"";
            for(size_t i=0;i<parts.size();++i){
                if(i>0) expr += " << ";
                if(isExpr[i]) expr += "(" + parts[i] + ")";
                else expr += "\"" + parts[i] + "\"";
            }
            return "std::cout << " + expr + ";";
        } else {
            return "std::cout << (" + s + ");";
        }
    }

    std::string transpile(const std::string& source){
        std::stringstream in(source); std::string line; std::vector<std::string> out;
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
        while(std::getline(in,line)){
            lineNo++; std::string L = trim_str(line);
            if(L.empty()) continue;
            if(L=="shuru" || L=="shesh") continue;
            while(!L.empty() && L[0]=='}'){ out.push_back("}"); L = trim_str(L.substr(1)); }
            if(L.empty()) continue;

            auto handleDecl = [&](const std::string& tkw){
                std::string cxxType = mapType(tkw);
                std::string rest = trim_str(L.substr(tkw.size()));
                if(!rest.empty() && rest[0]==' ') rest = trim_str(rest);
                std::string name, init; size_t eq = rest.find('=');
                if(eq==std::string::npos){ name = trim_str(rest); }
                else{ name = trim_str(rest.substr(0,eq)); init = trim_str(rest.substr(eq+1)); }
                if(!name.empty() && name.back()==';') name.pop_back();
                if(!init.empty() && init.back()==';') init.pop_back();
                if(!name.empty()) sym.declare(name,cxxType,lineNo);
                if(!init.empty()) sym.initialize(name);
                std::string stmt = cxxType + " " + name; if(!init.empty()) stmt += " = " + init; stmt += ";"; out.push_back(stmt); return true; };

            if(L.rfind("purno sonkha",0)==0){ if(handleDecl("purno sonkha")) continue; }
            if(L.rfind("dosomik sonkha",0)==0){ if(handleDecl("dosomik sonkha")) continue; }
            if(L.rfind("lekha",0)==0){ if(handleDecl("lekha")) continue; }
            if(L.rfind("akkhor",0)==0){ if(handleDecl("akkhor")) continue; }
            if(L.rfind("sotto-mittha",0)==0){ if(handleDecl("sotto-mittha")) continue; }

            if(L.rfind("poro",0)==0){
                size_t lp = L.find('('), rp=L.find(')');
                if(lp!=std::string::npos && rp!=std::string::npos && rp>lp){
                    std::string var = trim_str(L.substr(lp+1, rp-lp-1));
                    if(!var.empty() && var.back()==';') var.pop_back();
                    sym.initialize(var);
                    std::string type=""; if(sym.table.count(var)) type = sym.table[var].dtype;
                    std::string stmt; if(type=="std::string") stmt = "getline(cin >> ws, "+var+");"; else stmt = "cin >> "+var+";";
                    out.push_back(stmt); continue; }
            }

            if(L.rfind("dekhao",0)==0){
                std::string inside = trim_str(L.substr(std::string("dekhao").size()));
                if(!inside.empty() && inside[0]==' ') inside = trim_str(inside);
                if(!inside.empty() && inside.back()==';') inside.pop_back();
                out.push_back(renderDekhao(inside)); continue; }

            if(L.rfind("jodi",0)==0){ std::string rest = trim_str(L.substr(4)); out.push_back("if " + rest); continue; }
            if(L.rfind("nahoy jodi",0)==0){ std::string rest = trim_str(L.substr(std::string("nahoy jodi").size())); out.push_back("else if " + rest); continue; }
            if(L.rfind("nahoy",0)==0 && L.rfind("nahoy jodi",0)!=0){ std::string rest = trim_str(L.substr(std::string("nahoy").size())); if(rest.empty()) out.push_back("else"); else out.push_back(std::string("else ") + rest); continue; }

            if(L.rfind("ferot dao",0)==0){ std::string expr = trim_str(L.substr(std::string("ferot dao").size())); if(!expr.empty() && expr.back()==';') expr.pop_back(); out.push_back("return " + expr + ";"); continue; }

            if(L.rfind("loop",0)==0){ 
                std::string X=L; X = std::string("for") + L.substr(4);
                auto rep=[&](const std::string& a,const std::string& b){ size_t pos=0; while((pos=X.find(a,pos))!=std::string::npos){ X.replace(pos,a.size(),b); pos+=b.size(); } };
                rep("purno sonkha","int"); rep("dosomik sonkha","double"); rep("lekha","std::string"); rep("akkhor","char"); rep("sotto-mittha","bool");
                size_t lp = L.find('('), sc = L.find(';', lp==std::string::npos?0:lp);
                if(lp!=std::string::npos && sc!=std::string::npos && sc>lp){
                    std::string init = trim_str(L.substr(lp+1, sc-lp-1));
                    std::vector<std::pair<std::string,std::string>> types = {{"purno sonkha","int"},{"dosomik sonkha","double"},{"lekha","std::string"},{"akkhor","char"},{"sotto-mittha","bool"},{"int","int"},{"double","double"},{"std::string","std::string"},{"char","char"},{"bool","bool"}};
                    for(auto &p: types){ const std::string& t=p.first; const std::string& cxx=p.second; if(init.rfind(t,0)==0){ std::string rest = trim_str(init.substr(t.size())); if(!rest.empty() && rest[0]==' ') rest = trim_str(rest); std::string name = rest; size_t eq = rest.find('='); if(eq!=std::string::npos) name = trim_str(rest.substr(0,eq)); if(!name.empty()) sym.declare(name,cxx,lineNo), sym.initialize(name); break; } }
                }
                out.push_back(X); continue; 
            }

            { size_t eq = L.find('='); if(eq!=std::string::npos){ std::string lhs = trim_str(L.substr(0,eq)); if(!lhs.empty() && isalpha((unsigned char)lhs[0])){ sym.initialize(lhs); } } }
            out.push_back(L);
        }
        out.push_back("}");
        std::string code; for(auto&s:out){ code += s + "\n"; }
        return code;
    }
};
