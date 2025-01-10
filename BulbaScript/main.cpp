#include <io.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include "BS.hpp"
#include <codecvt>

int main(int argc, char* argv[]) {

    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);


    return BS::runScript(argc, argv);
}