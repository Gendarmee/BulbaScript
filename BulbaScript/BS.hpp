#pragma once

#include <string>
#include <iostream>
#include "RuntimeError.hpp"
#include "Interpritator.hpp"

using std::wstring;

class BS {
public:
    static void error(int line, wstring message);
    static int runScript(int argc, char* argv[]);
    static void runtimeError(RuntimeError error);
private:
    static std::wstring stringToWString(const std::string& str);
    static std::string wstringToString(const std::wstring& str);
    static Interpritator interpreter;
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(wstring path);
    static void runPrompt();
    static void run(wstring source);
    static void report(int line, wstring where, wstring message);
};