#include "Compiler.h"
#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "IRGenerator.h"
#include <iostream>
#include <sstream>

const char* tokenNames[] = {
    "INT", "FLOAT", "PRINT", "RETURN", "ID", "NUM", "ASSIGN", 
    "PLUS", "MINUS", "MUL", "DIV", "SEMI", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "EOF", "UNKNOWN"
};

void CompilerBackend::addError(string phase, string message, int line) {
    errors.push_back({phase, message, line});
}

string CompilerBackend::newTemp() {
    return "t" + to_string(++tempCounter);
}

void CompilerBackend::compile(const string& source) {
    Lexer lexer(*this);
    lexer.lex(source);

    Parser parser(*this);
    auto ast = parser.parse();

    SemanticAnalyzer semAnalyzer(*this);
    semAnalyzer.analyze(ast.get());

    IRGenerator irGen(*this);
    irGen.generate(ast.get());
}

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

string CompilerBackend::toJSON() {
    ostringstream out;
    out << "{";
    out << "\"tokens\": [";
    for (size_t i = 0; i < tokens.size(); i++) {
        out << "{\"type\": \"" << tokenNames[tokens[i].type] << "\", \"value\": \"" << escapeJson(tokens[i].value) << "\", \"line\": " << tokens[i].line << "}";
        if (i < tokens.size() - 1) out << ",";
    }
    out << "],";

    out << "\"symbols\": [";
    size_t symIdx = 0;
    for (auto const& [key, val] : symbolTable) {
        out << "{\"name\": \"" << escapeJson(key) << "\", \"type\": \"" << escapeJson(val) << "\"}";
        if (symIdx < symbolTable.size() - 1) out << ",";
        symIdx++;
    }
    out << "],";

    out << "\"ir\": [";
    for (size_t i = 0; i < irCodes.size(); i++) {
        out << "\"" << escapeJson(irCodes[i]) << "\"";
        if (i < irCodes.size() - 1) out << ",";
    }
    out << "],";

    out << "\"errors\": [";
    for (size_t i = 0; i < errors.size(); i++) {
        out << "{\"phase\": \"" << escapeJson(errors[i].phase) << "\", \"message\": \"" << escapeJson(errors[i].message) << "\", \"line\": " << errors[i].line << "}";
        if (i < errors.size() - 1) out << ",";
    }
    out << "]";
    out << "}\n";
    return out.str();
}
