#pragma once
#include <string>

using namespace std;

enum TokenType {
    T_INT, T_FLOAT, T_PRINT, T_RETURN, T_ID, T_NUM, T_ASSIGN, 
    T_PLUS, T_MINUS, T_MUL, T_DIV, T_SEMI, T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_EOF, T_UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int line;
};

struct Error {
    string phase;
    string message;
    int line;
};
