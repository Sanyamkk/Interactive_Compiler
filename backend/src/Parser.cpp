#include "Parser.h"

Token Parser::peek() {
    if (currentToken < backend.tokens.size()) return backend.tokens[currentToken];
    return backend.tokens.back(); // EOF
}

void Parser::consume() {
    if (currentToken < backend.tokens.size()) currentToken++;
}

bool Parser::match(TokenType t) {
    if (peek().type == t) {
        consume();
        return true;
    }
    return false;
}

void Parser::panicToSemi() {
    while (peek().type != T_SEMI && peek().type != T_EOF && peek().type != T_RBRACE) {
        consume();
    }
    if (peek().type == T_SEMI || peek().type == T_RBRACE) consume();
}

unique_ptr<Expr> Parser::parsePrimary() {
    Token t = peek();
    if (t.type == T_ID || t.type == T_NUM) {
        consume();
        if (t.type == T_ID) return make_unique<IdentifierExpr>(t.value, t.line);
        else return make_unique<NumberExpr>(t.value, t.line);
    }
    if (t.type == T_LPAREN) {
        consume();
        auto expr = parseExpression();
        if (match(T_RPAREN)) return expr;
        backend.addError("Syntax", "Expected ')'", peek().line);
        return expr;
    }
    backend.addError("Syntax", "Expected expression", t.line);
    return nullptr;
}

unique_ptr<Expr> Parser::parseTerm() {
    auto left = parsePrimary();
    while (left) {
        Token op = peek();
        if (op.type == T_MUL || op.type == T_DIV) {
            consume();
            auto right = parsePrimary();
            if (!right) {
                backend.addError("Syntax", "Expected operand after operator", op.line);
                break;
            }
            left = make_unique<BinOpExpr>(move(left), op, move(right));
        } else {
            break;
        }
    }
    return left;
}

unique_ptr<Expr> Parser::parseExpression() {
    auto left = parseTerm();
    while (left) {
        Token op = peek();
        if (op.type == T_PLUS || op.type == T_MINUS) {
            consume();
            auto right = parseTerm();
            if (!right) {
                backend.addError("Syntax", "Expected operand after operator", op.line);
                break;
            }
            left = make_unique<BinOpExpr>(move(left), op, move(right));
        } else {
            break;
        }
    }
    return left;
}

unique_ptr<Stmt> Parser::parseStatement() {
    Token t = peek();
    if (t.type == T_INT || t.type == T_FLOAT) {
        consume();
        Token id = peek();
        if (match(T_ID)) {
            // Function detection (like int main())
            if (peek().type == T_LPAREN) {
                consume(); // consume '('
                if (!match(T_RPAREN)) {
                    backend.addError("Syntax", "Expected ')' after function name", peek().line);
                    panicToSemi();
                    return nullptr;
                }
                if (!match(T_LBRACE)) {
                    backend.addError("Syntax", "Expected '{' to start function body", peek().line);
                }
                return make_unique<FunctionStmt>(t.value, id.value, id.line);
            }

            unique_ptr<Expr> initExpr = nullptr;
            if (match(T_ASSIGN)) {
                initExpr = parseExpression();
            }
            if (!match(T_SEMI)) {
                backend.addError("Syntax", "Missing ';' after declaration", peek().line);
                panicToSemi();
            }
            return make_unique<VarDeclStmt>(t.value, id.value, move(initExpr), id.line);
        } else {
            backend.addError("Syntax", "Expected identifier after type", t.line);
            panicToSemi();
            return nullptr;
        }
    } else if (t.type == T_ID) {
        consume();
        if (match(T_ASSIGN)) {
            unique_ptr<Expr> exprResult = parseExpression();
            if (!match(T_SEMI)) {
                backend.addError("Syntax", "Missing ';' after assignment", peek().line);
                panicToSemi();
            }
            return make_unique<AssignStmt>(t.value, move(exprResult), t.line);
        } else {
            backend.addError("Syntax", "Expected '=' after identifier", t.line);
            panicToSemi();
            return nullptr;
        }
    } else if (t.type == T_PRINT) {
        consume();
        if (match(T_LPAREN)) {
            Token id = peek();
            if (match(T_ID)) {
                if (!match(T_RPAREN)) backend.addError("Syntax", "Missing ')'", peek().line);
                if (!match(T_SEMI)) {
                    backend.addError("Syntax", "Missing ';' after print", peek().line);
                    panicToSemi();
                }
                return make_unique<PrintStmt>(id.value, id.line);
            } else {
                backend.addError("Syntax", "Expected identifier in print", peek().line);
                panicToSemi();
                return nullptr;
            }
        } else {
            backend.addError("Syntax", "Expected '(' after print", peek().line);
            panicToSemi();
            return nullptr;
        }
    } else if (t.type == T_RETURN) {
        consume();
        unique_ptr<Expr> exprResult = parseExpression();
        if (!match(T_SEMI)) {
            backend.addError("Syntax", "Missing ';' after return", peek().line);
            panicToSemi();
        }
        return make_unique<ReturnStmt>(move(exprResult), t.line);
    } else if (t.type == T_RBRACE) {
        consume(); // End of block like }
        return nullptr;
    } else {
        backend.addError("Syntax", "Unexpected token '" + t.value + "'", t.line);
        consume(); // prevent infinite loop
        panicToSemi();
        return nullptr;
    }
}

unique_ptr<Program> Parser::parse() {
    auto prog = make_unique<Program>();
    while (peek().type != T_EOF) {
        if (auto stmt = parseStatement()) {
            prog->statements.push_back(move(stmt));
        }
    }
    return prog;
}
