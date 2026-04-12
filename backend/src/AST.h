#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Types.h"

using namespace std;

// Base Node
struct ASTNode {
    virtual ~ASTNode() = default;
};

// Expressions
struct Expr : public ASTNode {
    virtual ~Expr() = default;
};

struct IdentifierExpr : public Expr {
    string name;
    int line;
    IdentifierExpr(string name, int line) : name(name), line(line) {}
};

struct NumberExpr : public Expr {
    string value;
    int line;
    NumberExpr(string val, int line) : value(val), line(line) {}
};

struct BinOpExpr : public Expr {
    unique_ptr<Expr> left;
    Token op;
    unique_ptr<Expr> right;
    BinOpExpr(unique_ptr<Expr> left, Token op, unique_ptr<Expr> right)
        : left(move(left)), op(op), right(move(right)) {}
};

// Statements
struct Stmt : public ASTNode {
    virtual ~Stmt() = default;
};

struct VarDeclStmt : public Stmt {
    string typeName;
    string idName;
    unique_ptr<Expr> initExpr; // nullable
    int line;
    VarDeclStmt(string typeName, string idName, unique_ptr<Expr> initExpr, int line)
        : typeName(typeName), idName(idName), initExpr(move(initExpr)), line(line) {}
};

struct AssignStmt : public Stmt {
    string idName;
    unique_ptr<Expr> expr;
    int line;
    AssignStmt(string idName, unique_ptr<Expr> expr, int line)
        : idName(idName), expr(move(expr)), line(line) {}
};

struct PrintStmt : public Stmt {
    string idName;
    int line;
    PrintStmt(string idName, int line) : idName(idName), line(line) {}
};

struct ReturnStmt : public Stmt {
    unique_ptr<Expr> expr;
    int line;
    ReturnStmt(unique_ptr<Expr> expr, int line) : expr(move(expr)), line(line) {}
};

struct FunctionStmt : public Stmt {
    string name;
    string returnType;
    int line;
    FunctionStmt(string returnType, string name, int line)
        : returnType(returnType), name(name), line(line) {}
};

struct Program : public ASTNode {
    vector<unique_ptr<Stmt>> statements;
};
