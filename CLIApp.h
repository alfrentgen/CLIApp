#ifndef CLIAPP_H
#define CLIAPP_H

#include <memory>
#include "IApp.h"

std::unique_ptr<IApp> makeApp(int argc, const char** argv);

#endif //CLIAPP_H