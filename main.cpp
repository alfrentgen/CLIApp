#include <iostream>
#include "CLIApp.h"

int main(int argc, const char** argv)
{
    try {
        makeApp(--argc, ++argv)->run();
    }
    catch (const std::runtime_error& re) {
        std::cout << re.what() << std::endl;
        return -1;
    }
    catch (...) {
        return -1;
    }
    return 0;
}
