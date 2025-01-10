#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "BS.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"

using std::wstring;
using std::wstringstream;
using std::ifstream;
using std::wcin;
using std::wcout;
using std::endl;
using std::vector;

bool BS::hadError = false;
Interpritator BS::interpreter;
bool BS::hadRuntimeError = false;

int BS::runScript(int argc, char* argv[]) {
    if (argc > 2) {
        wprintf(L"Usage: BulbaScript \n");
        return 1;
    }
    else if (argc == 2) {
        runFile(stringToWString(argv[1]));
    }
    else {
        runPrompt();
    }
    return 0;
}

std::wstring BS::stringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

std::string BS::wstringToString(const std::wstring& str) {
    return std::string(str.begin(), str.end());
}


void BS::runtimeError(RuntimeError error) {
    std::wcout << error.getMessage() <<
        L"\n[line " << error.token.line << L"]";
        hadRuntimeError = true;
}

void BS::runFile( std::wstring path) {
    std::wifstream input(path);
    if (!input) {
        std::wcerr << L"Could not open file: " << path << std::endl;
        exit(1);
    }

    std::wstringstream sstr;
    sstr << input.rdbuf();

    //std::wcout << L"File content: " << sstr.str() << std::endl;

    run(sstr.str());

    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}

void BS::runPrompt() {
    wstring input;
    while (true) {
        wcout << L"> ";
        getline(wcin, input);
        run(input);
        hadError = false;
    }
}

void BS::error(int line, wstring message) {
    report(line, L"", message);
}

void BS::report(int line, wstring where, wstring message) {
    wprintf(L"[line %d] Error%s: %s\n", line, where.c_str(), message.c_str());
    hadError = true;
}

void BS::run(wstring source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (hadError) {
        std::wcerr << L"Parsing error occurred. Expression is invalid." << std::endl;
        return;
    }

    interpreter.interpret(statements);

//AstPrinter printer;
//std::wstring output = printer.print(statements);
//std::wcout << output << std::endl;
}