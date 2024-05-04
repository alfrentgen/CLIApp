#include <iostream>
#include "CLIApp.h"

int main(int argc, const char** argv) {
    return makeApp(--argc, ++argv)->run();
}
