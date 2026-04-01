#include<bits/stdc++.h>
using namespace std;

enum TokenType {
    T_INT, T_FLOAT, T_PRINT, T_RETURN, T_ID, T_NUM, T_ASSIGN, 
    T_PLUS, T_MINUS, T_MUL, T_DIV, T_SEMI, T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_EOF, T_UNKNOWN
};

string tokenNames[] = {
    "INT", "FLOAT", "PRINT", "RETURN", "ID", "NUM", "ASSIGN", 
    "PLUS", "MINUS", "MUL", "DIV", "SEMI", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "EOF", "UNKNOWN"
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

vector<Token> tokens;
vector<Error> errors;
map<string, string> symbolTable;
vector<string> irCodes;
size_t currentToken = 0;
int tempCounter = 0;

void addError(string phase, string message, int line) {
    errors.push_back({phase, message, line});
}

// ----------------- LEXER -----------------
void lex(const string& source) {
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
            if (id == "int") tokens.push_back({T_INT, id, line});
            else if (id == "float") tokens.push_back({T_FLOAT, id, line});
            else if (id == "print" || id == "printf") tokens.push_back({T_PRINT, id, line});
            else if (id == "return") tokens.push_back({T_RETURN, id, line});
            else tokens.push_back({T_ID, id, line});
        }
        else if (isdigit(c)) {
            string num = "";
            while (i < source.length() && (isdigit(source[i]) || source[i] == '.')) {
                num += source[i++];
            }
            i--;
            tokens.push_back({T_NUM, num, line});
        }
        else {
            switch(c) {
                case '=': tokens.push_back({T_ASSIGN, "=", line}); break;
                case '+': tokens.push_back({T_PLUS, "+", line}); break;
                case '-': tokens.push_back({T_MINUS, "-", line}); break;
                case '*': tokens.push_back({T_MUL, "*", line}); break;
                case '/': tokens.push_back({T_DIV, "/", line}); break;
                case ';': tokens.push_back({T_SEMI, ";", line}); break;
                case '(': tokens.push_back({T_LPAREN, "(", line}); break;
                case ')': tokens.push_back({T_RPAREN, ")", line}); break;
                case '{': tokens.push_back({T_LBRACE, "{", line}); break;
                case '}': tokens.push_back({T_RBRACE, "}", line}); break;
                default: 
                    tokens.push_back({T_UNKNOWN, string(1, c), line});
                    addError("Lexical", "Unknown character '" + string(1, c) + "'", line);
            }
        }
    }
    tokens.push_back({T_EOF, "", line});
}

// ----------------- PARSER & SEMANTIC & IR -----------------
string newTemp() {
    return "t" + to_string(++tempCounter);
}

Token peek() {
    if (currentToken < tokens.size()) return tokens[currentToken];
    return tokens.back();
}

void consume() {
    if (currentToken < tokens.size()) currentToken++;
}

bool match(TokenType t) {
    if (peek().type == t) {
        consume();
        return true;
    }
    return false;
}

void panicToSemi() {
    while (peek().type != T_SEMI && peek().type != T_EOF && peek().type != T_RBRACE) {
        consume();
    }
    if (peek().type == T_SEMI || peek().type == T_RBRACE) consume();
}

string parseExpression() {
    Token t1 = peek();
    if (t1.type == T_ID || t1.type == T_NUM) {
        consume();
        if (t1.type == T_ID && symbolTable.find(t1.value) == symbolTable.end()) {
            addError("Semantic", "Undeclared variable '" + t1.value + "' used in expression", t1.line);
        }
        
        Token op = peek();
        if (op.type == T_PLUS || op.type == T_MINUS || op.type == T_MUL || op.type == T_DIV) {
            consume();
            Token t2 = peek();
            if (t2.type == T_ID || t2.type == T_NUM) {
                consume();
                 if (t2.type == T_ID && symbolTable.find(t2.value) == symbolTable.end()) {
                    addError("Semantic", "Undeclared variable '" + t2.value + "' used in expression", t2.line);
                }
                string t = newTemp();
                irCodes.push_back(t + " = " + t1.value + " " + op.value + " " + t2.value);
                return t;
            } else {
                addError("Syntax", "Expected operand after operator", op.line);
                return t1.value;
            }
        }
        return t1.value;
    }
    addError("Syntax", "Expected expression", peek().line);
    return "";
}

void parseStatement() {
    Token t = peek();
    if (t.type == T_INT || t.type == T_FLOAT) {
        consume();
        Token id = peek();
        if (match(T_ID)) {
            // Function detection (like int main())
            if (peek().type == T_LPAREN) {
                consume(); // consume '('
                if (!match(T_RPAREN)) {
                    addError("Syntax", "Expected ')' after function name", peek().line);
                    panicToSemi();
                }
                if (!match(T_LBRACE)) {
                    addError("Syntax", "Expected '{' to start function body", peek().line);
                }
                irCodes.push_back("FUNC " + id.value + ":");
                return; // Parsing body will continue as standard statements
            }

            if (symbolTable.find(id.value) != symbolTable.end()) {
                addError("Semantic", "Variable '" + id.value + "' already declared", id.line);
            } else {
                symbolTable[id.value] = t.value;
            }
            if (match(T_ASSIGN)) {
                string exprResult = parseExpression();
                irCodes.push_back(id.value + " = " + exprResult);
            }
            if (!match(T_SEMI)) {
                addError("Syntax", "Missing ';' after declaration", peek().line);
                panicToSemi();
            }
        } else {
            addError("Syntax", "Expected identifier after type", t.line);
            panicToSemi();
        }
    } else if (t.type == T_ID) {
        consume();
        if (symbolTable.find(t.value) == symbolTable.end()) {
            addError("Semantic", "Undeclared variable '" + t.value + "' assigned", t.line);
        }
        if (match(T_ASSIGN)) {
            string exprResult = parseExpression();
            irCodes.push_back(t.value + " = " + exprResult);
            if (!match(T_SEMI)) {
                addError("Syntax", "Missing ';' after assignment", peek().line);
                panicToSemi();
            }
        } else {
            addError("Syntax", "Expected '=' after identifier", t.line);
            panicToSemi();
        }
    } else if (t.type == T_PRINT) {
        consume();
        if (match(T_LPAREN)) {
            Token id = peek();
            if (match(T_ID)) {
                if (symbolTable.find(id.value) == symbolTable.end()) {
                    addError("Semantic", "Undeclared variable '" + id.value + "' in print", id.line);
                }
                irCodes.push_back("PRINT " + id.value);
                if (!match(T_RPAREN)) addError("Syntax", "Missing ')'", peek().line);
                if (!match(T_SEMI)) {
                    addError("Syntax", "Missing ';' after print", peek().line);
                    panicToSemi();
                }
            } else {
                addError("Syntax", "Expected identifier in print", peek().line);
                panicToSemi();
            }
        } else {
            addError("Syntax", "Expected '(' after print", peek().line);
            panicToSemi();
        }
    } else if (t.type == T_RETURN) {
        consume();
        string exprResult = parseExpression();
        irCodes.push_back("RETURN " + exprResult);
        if (!match(T_SEMI)) {
            addError("Syntax", "Missing ';' after return", peek().line);
            panicToSemi();
        }
    } else if (t.type == T_RBRACE) {
        consume(); // End of a block like }
    } else {
        addError("Syntax", "Unexpected token '" + t.value + "'", t.line);
        consume(); // prevent infinite loop
        panicToSemi();
    }
}

void parse() {
    while (peek().type != T_EOF) {
        parseStatement();
    }
}

// ----------------- JSON OUTPUT -----------------
string escapeJson(const string& s) {
    ostringstream o;
    for (char c : s) {
        if (c == '"') o << "\\\"";
        else if (c == '\\') o << "\\\\";
        else if (c == '\n') o << "\\n";
        else o << c;
    }
    return o.str();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "{\"error\": \"No input file\"}" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cout << "{\"error\": \"Could not open file\"}" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();

    lex(source);
    parse();

    cout << "{";
    cout << "\"tokens\": [";
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "{\"type\": \"" << tokenNames[tokens[i].type] << "\", \"value\": \"" << escapeJson(tokens[i].value) << "\", \"line\": " << tokens[i].line << "}";
        if (i < tokens.size() - 1) cout << ",";
    }
    cout << "],";

    cout << "\"symbols\": [";
    size_t symIdx = 0;
    for (auto const& [key, val] : symbolTable) {
        cout << "{\"name\": \"" << escapeJson(key) << "\", \"type\": \"" << escapeJson(val) << "\"}";
        if (symIdx < symbolTable.size() - 1) cout << ",";
        symIdx++;
    }
    cout << "],";

    cout << "\"ir\": [";
    for (size_t i = 0; i < irCodes.size(); i++) {
        cout << "\"" << escapeJson(irCodes[i]) << "\"";
        if (i < irCodes.size() - 1) cout << ",";
    }
    cout << "],";

    cout << "\"errors\": [";
    for (size_t i = 0; i < errors.size(); i++) {
        cout << "{\"phase\": \"" << escapeJson(errors[i].phase) << "\", \"message\": \"" << escapeJson(errors[i].message) << "\", \"line\": " << errors[i].line << "}";
        if (i < errors.size() - 1) cout << ",";
    }
    cout << "]";

    cout << "}\n";
    return 0;
}
