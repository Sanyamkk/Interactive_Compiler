#pragma once
#include "Compiler.h"
#include "AST.h"
#include <string>

using namespace std;

class IRGenerator {
    CompilerBackend& backend;

    string generateExpr(Expr* expr);
    void generateStmt(Stmt* stmt);
public:
    IRGenerator(CompilerBackend& backend) : backend(backend) {}
    void generate(Program* prog);
};
