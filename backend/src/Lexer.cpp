#include "Lexer.h"
#include <cctype>

void Lexer::lex(const string& source) {
    int line = 1;
    for (size_t i = 0; i < source.length(); i++) {
        char c = source[i];
        
        // Skip preprocessor directives like #include <stdio.h>
        if (c == '#') { 
            while (i < source.length() && source[i] != '\n') i++;
            line++;
            continue;
        }
        
        if (isspace(c)) {
            if (c == '\n') line++;
            continue;
        }
        
        if (isalpha(c)) {
            string id = "";
            while (i < source.length() && (isalnum(source[i]) || source[i] == '_')) {
                id += source[i++];
            }
            i--; // back up
            if (id == "int") backend.tokens.push_back({T_INT, id, line});
            else if (id == "float") backend.tokens.push_back({T_FLOAT, id, line});
            else if (id == "print" || id == "printf") backend.tokens.push_back({T_PRINT, id, line});
            else if (id == "return") backend.tokens.push_back({T_RETURN, id, line});
            else backend.tokens.push_back({T_ID, id, line});
        }
        else if (isdigit(c)) {
            string num = "";
            while (i < source.length() && (isdigit(source[i]) || source[i] == '.')) {
                num += source[i++];
            }
            i--;
            backend.tokens.push_back({T_NUM, num, line});
        }
        else {
            switch(c) {
                case '=': backend.tokens.push_back({T_ASSIGN, "=", line}); break;
                case '+': backend.tokens.push_back({T_PLUS, "+", line}); break;
                case '-': backend.tokens.push_back({T_MINUS, "-", line}); break;
                case '*': backend.tokens.push_back({T_MUL, "*", line}); break;
                case '/': backend.tokens.push_back({T_DIV, "/", line}); break;
                case ';': backend.tokens.push_back({T_SEMI, ";", line}); break;
                case '(': backend.tokens.push_back({T_LPAREN, "(", line}); break;
                case ')': backend.tokens.push_back({T_RPAREN, ")", line}); break;
                case '{': backend.tokens.push_back({T_LBRACE, "{", line}); break;
                case '}': backend.tokens.push_back({T_RBRACE, "}", line}); break;
                default: 
                    backend.tokens.push_back({T_UNKNOWN, string(1, c), line});
                    backend.addError("Lexical", "Unknown character '" + string(1, c) + "'", line);
            }
        }
    }
    backend.tokens.push_back({T_EOF, "", line});
}
