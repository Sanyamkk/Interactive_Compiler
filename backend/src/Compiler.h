#pragma once
#include <vector>
#include <map>
#include <string>
#include "Types.h"

using namespace std;

class CompilerBackend {
public:
    vector<Token> tokens;
    vector<Error> errors;
    map<string, string> symbolTable;
    vector<string> irCodes;
    int tempCounter = 0;

    void addError(string phase, string message, int line);
    string newTemp();
    
    void compile(const string& source);
    string toJSON();
};
