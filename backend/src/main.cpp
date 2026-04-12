#include <iostream>
#include <fstream>
#include <sstream>
#include "Compiler.h"

using namespace std;

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

    CompilerBackend compiler;
    compiler.compile(source);

    cout << compiler.toJSON() << endl;

    return 0;
}
