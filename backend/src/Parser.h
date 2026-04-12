#pragma once
#include "Compiler.h"
#include "AST.h"
#include <memory>

using namespace std;

class Parser {
    CompilerBackend& backend;
    size_t currentToken = 0;

    Token peek();
    void consume();
    bool match(TokenType t);
    void panicToSemi();
    
    unique_ptr<Expr> parseExpression();
    unique_ptr<Expr> parseTerm();
    unique_ptr<Expr> parsePrimary();
    unique_ptr<Stmt> parseStatement();
    
public:
    Parser(CompilerBackend& backend) : backend(backend) {}
    unique_ptr<Program> parse();
};
