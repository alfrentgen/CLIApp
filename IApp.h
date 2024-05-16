#ifndef CLIAPP_IAPP_H
#define CLIAPP_IAPP_H

class IApp {
public:
    virtual int run() = 0;
    virtual ~IApp() = default;
};

#endif //CLIAPP_IAPP_H
