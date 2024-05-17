#include <iostream>
#include "CLIApp.h"

int main(int argc, const char** argv)
{
    try {
        makeApp(--argc, ++argv)->run();
    }
    catch (...) {
        return -1;
    }
    return 0;
}
