#include "SemanticAnalyzer.h"

void SemanticAnalyzer::analyzeExpr(Expr* expr) {
    if (!expr) return;
    if (auto idExpr = dynamic_cast<IdentifierExpr*>(expr)) {
        if (backend.symbolTable.find(idExpr->name) == backend.symbolTable.end()) {
            backend.addError("Semantic", "Undeclared variable '" + idExpr->name + "' used in expression", idExpr->line);
        }
    } else if (auto binOp = dynamic_cast<BinOpExpr*>(expr)) {
        analyzeExpr(binOp->left.get());
        analyzeExpr(binOp->right.get());
    }
}

void SemanticAnalyzer::analyzeStmt(Stmt* stmt) {
    if (!stmt) return;
    if (auto varDecl = dynamic_cast<VarDeclStmt*>(stmt)) {
        if (backend.symbolTable.find(varDecl->idName) != backend.symbolTable.end()) {
            backend.addError("Semantic", "Variable '" + varDecl->idName + "' already declared", varDecl->line);
        } else {
            backend.symbolTable[varDecl->idName] = varDecl->typeName;
        }
        analyzeExpr(varDecl->initExpr.get());
    } else if (auto assignSt = dynamic_cast<AssignStmt*>(stmt)) {
        if (backend.symbolTable.find(assignSt->idName) == backend.symbolTable.end()) {
            backend.addError("Semantic", "Undeclared variable '" + assignSt->idName + "' assigned", assignSt->line);
        }
        analyzeExpr(assignSt->expr.get());
    } else if (auto printSt = dynamic_cast<PrintStmt*>(stmt)) {
        if (backend.symbolTable.find(printSt->idName) == backend.symbolTable.end()) {
            backend.addError("Semantic", "Undeclared variable '" + printSt->idName + "' in print", printSt->line);
        }
    } else if (auto returnSt = dynamic_cast<ReturnStmt*>(stmt)) {
        analyzeExpr(returnSt->expr.get());
    }
}

void SemanticAnalyzer::analyze(Program* prog) {
    for (auto& stmt : prog->statements) {
        analyzeStmt(stmt.get());
    }
}
