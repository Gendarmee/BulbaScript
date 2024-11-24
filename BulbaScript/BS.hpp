#pragma once

#include <string>
#include <iostream>
#include "RuntimeError.hpp"
#include "Interpreter.hpp"

class Interpritator;

using std::wstring;

class BS {
public:
    static void error(int line, wstring message);
    static int runScript(int argc, wchar_t* argv[]);
    static void runtimeError(RuntimeError error);
private:
    static Interpritator interpreter;
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(wstring path);
    static void runPrompt();
    static void run(wstring source);
    static void report(int line, wstring where, wstring message);
};