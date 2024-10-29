#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "BS.hpp"
#include "Scanner.hpp"

using std::wstring;
using std::wstringstream;
using std::ifstream;
using std::wcin;
using std::wcout;
using std::endl;
using std::vector;

bool BS::hadError = false;
bool BS::hadRuntimeError = false;

int BS::runScript(int argc, wchar_t* argv[]) {
    if (argc > 2) {
        wprintf(L"Usage: BulbaScript \n");
        return 1;
    }
    else if (argc == 2) {
        runFile(argv[1]);
    }
    else {
        runPrompt();
    }
    return 0;
}

void BS::runFile(wstring path) {
    ifstream input(path);
    if (!input) {
        wprintf(L"Could not open file: %s\n", path.c_str());
        exit(1);
    }
    wstringstream sstr;
    sstr << input.rdbuf();
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
    vector<Token> tokens = scanner.scanTokens(); 
    for (auto token : tokens) {
        wcout << token.toString() << endl;
    }
}