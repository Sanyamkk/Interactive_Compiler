#pragma once
#include "Compiler.h"
#include <string>

using namespace std;

class Lexer {
    CompilerBackend& backend;
public:
    Lexer(CompilerBackend& backend) : backend(backend) {}
    void lex(const string& source);
};
