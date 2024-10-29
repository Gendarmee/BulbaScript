#include <io.h>
#include <fcntl.h>
#include "BS.hpp"

int main(int argc, wchar_t* argv[]) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    return BS::runScript(argc, argv);
}