#include "IRGenerator.h"

string IRGenerator::generateExpr(Expr* expr) {
    if (!expr) return "";
    if (auto idExpr = dynamic_cast<IdentifierExpr*>(expr)) {
        return idExpr->name;
    } else if (auto numExpr = dynamic_cast<NumberExpr*>(expr)) {
        return numExpr->value;
    } else if (auto binOp = dynamic_cast<BinOpExpr*>(expr)) {
        string left = generateExpr(binOp->left.get());
        string op = binOp->op.value;
        string right = generateExpr(binOp->right.get());
        string t = backend.newTemp();
        backend.irCodes.push_back(t + " = " + left + " " + op + " " + right);
        return t;
    }
    return "";
}

void IRGenerator::generateStmt(Stmt* stmt) {
    if (!stmt) return;
    if (auto varDecl = dynamic_cast<VarDeclStmt*>(stmt)) {
        if (varDecl->initExpr) {
            string exprRes = generateExpr(varDecl->initExpr.get());
            backend.irCodes.push_back(varDecl->idName + " = " + exprRes);
        }
    } else if (auto assignSt = dynamic_cast<AssignStmt*>(stmt)) {
        string exprRes = generateExpr(assignSt->expr.get());
        backend.irCodes.push_back(assignSt->idName + " = " + exprRes);
    } else if (auto printSt = dynamic_cast<PrintStmt*>(stmt)) {
        backend.irCodes.push_back("PRINT " + printSt->idName);
    } else if (auto returnSt = dynamic_cast<ReturnStmt*>(stmt)) {
        string exprRes = generateExpr(returnSt->expr.get());
        backend.irCodes.push_back("RETURN " + exprRes);
    } else if (auto funcSt = dynamic_cast<FunctionStmt*>(stmt)) {
        backend.irCodes.push_back("FUNC " + funcSt->name + ":");
    }
}

void IRGenerator::generate(Program* prog) {
    for (auto& stmt : prog->statements) {
        generateStmt(stmt.get());
    }
}
