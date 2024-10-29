#pragma once

#include <string>
#include <iostream>

using std::wstring;

class BS {
public:
    static void error(int line, wstring message);
    static int runScript(int argc, wchar_t* argv[]);
private:
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(wstring path);
    static void runPrompt();
    static void run(wstring source);
    static void report(int line, wstring where, wstring message);
};