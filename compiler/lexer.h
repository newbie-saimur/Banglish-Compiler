#pragma once
#include <string>
#include <vector>
#include <set>
#include <cctype>
#include "token.h"

static const std::set<std::string> BG_KEYWORDS = {
    "shuru","shesh","purno","sonkha","dosomik","lekha","akkhor","sotto-mittha",
    "jodi","nahoy","poro","dekhao","loop","ferot","dao"
};

static const std::set<std::string> BG_SIMPLE_OPS = {
    "+","-","*","/","%","++","--","+=","-=","*=","/=",
    "<=",">=","<",">","!","==","!=","=",
    "&&","||","&","|",
    "(",")","{","}",";","," 
};

struct Lexer {
    std::string src; std::vector<Token> tokens; int i=0; int line=1; int col=1;
    Lexer(const std::string&s):src(s){}

    char peek(int k=0){ if(i+k<(int)src.size()) return src[i+k]; return '\0'; }
    char get(){ char c=peek(); if(c=='\n'){ line++; col=1; } else col++; i++; return c; }
    void add(const std::string&type,const std::string&lex,int l,int c){ tokens.push_back({type,lex,l,c}); }

    static bool isIdentStart(char c){ return isalpha((unsigned char)c) || c=='_'; }
    static bool isIdentChar(char c){ return isalnum((unsigned char)c) || c=='_'; }

    void lex(){
        while(true){
            char c=peek(); if(c=='\0') break;
            if(isspace((unsigned char)c)){ get(); continue; }
            int l=line, c0=col;
            if(c=='/' && peek(1)=='/') { while(peek()!='\n' && peek()!='\0') get(); continue; }
            if(c=='"'){
                get(); std::string val; bool terminated = false;
                while(true){ 
                    char d=peek(); 
                    if(d=='\0' || d=='\n') { 
                        break; 
                    }
                    if(d=='\\'){ 
                        get(); 
                        char e=get(); 
                        if(e != '\0') {
                            val.push_back('\\'); 
                            val.push_back(e); 
                        }
                    }
                    else if(d=='"'){ 
                        get(); 
                        terminated = true;
                        break; 
                    }
                    else { 
                        val.push_back(get()); 
                    }
                }
                if(terminated) {
                    add("STRING","\""+val+"\"",l,c0); 
                } else {
                    add("ERROR","UNCLOSED_STRING",l,c0);
                }
                continue;
            }
            if(isdigit((unsigned char)c)){
                std::string num; bool hasDot=false;
                while(isdigit((unsigned char)peek()) || (!hasDot && peek()=='.')){
                    if(peek()=='.') {
                        hasDot=true;
                    }
                    num.push_back(get());
                }
                add("NUMBER",num,l,c0); continue;
            }
            if(isIdentStart(c)){
                std::string id; id.push_back(get());
                while(isIdentChar(peek())) id.push_back(get());
                int j=i; while(isspace((unsigned char)src[j]) && src[j]!='\n') j++;
                if(j<(int)src.size() && (isalpha((unsigned char)src[j])||src[j]=='_')){
                    int k=j; std::string id2; id2.push_back(src[k++]);
                    while(k<(int)src.size() && (isalnum((unsigned char)src[k])||src[k]=='_')) id2.push_back(src[k++]);
                    std::string combo = id + std::string(" ") + id2;
                    if(combo=="purno sonkha" || combo=="dosomik sonkha" || combo=="ferot dao" || combo=="nahoy jodi"){
                        while(i<k) { get(); }
                        add("KEYWORD",combo,l,c0); continue;
                    }
                }
                if(peek()== '-'){
                    int j=i+1;
                    if(j<(int)src.size() && (isalpha((unsigned char)src[j])||src[j]=='_')){
                        int k=j; std::string id2; id2.push_back(src[k++]);
                        while(k<(int)src.size() && (isalnum((unsigned char)src[k])||src[k]=='_')) id2.push_back(src[k++]);
                        std::string combo = id + std::string("-") + id2;
                        if(combo=="sotto-mittha"){
                            while(i<k) { get(); }
                            add("KEYWORD",combo,l,c0); continue;
                        }
                    }
                }
                if(BG_KEYWORDS.count(id)) add("KEYWORD",id,l,c0);
                else add("IDENT",id,l,c0);
                continue;
            }
            std::string two; two.push_back(c); two.push_back(peek(1));
            if(BG_SIMPLE_OPS.count(two)) { get(); get(); add("OP",two,l,c0); continue; }
            std::string one; one.push_back(c);
            if(BG_SIMPLE_OPS.count(one)) { get(); add("OP",one,l,c0); continue; }
            get(); add("OP",std::string(1,c),l,c0);
        }
        add("EOF","",line,col);
    }
};
