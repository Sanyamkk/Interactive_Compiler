@echo off
cd src
echo Compiling backend files...
g++ main.cpp Compiler.cpp Lexer.cpp Parser.cpp SemanticAnalyzer.cpp IRGenerator.cpp -o ../compiler.exe
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)
echo Build successful!
cd ..
