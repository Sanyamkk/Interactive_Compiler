#pragma once
#include "Compiler.h"
#include "AST.h"

class SemanticAnalyzer {
    CompilerBackend& backend;

    void analyzeExpr(Expr* expr);
    void analyzeStmt(Stmt* stmt);
public:
    SemanticAnalyzer(CompilerBackend& backend) : backend(backend) {}
    void analyze(Program* prog);
};
