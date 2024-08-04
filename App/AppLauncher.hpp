#pragma once

#include <memory>

namespace App {

class AppLauncher
{
public:
    AppLauncher();
    ~AppLauncher();

    static std::unique_ptr<AppLauncher> create();

    int start(int argc, char** argv);
    void destroy();

protected:
    void initializeQml();
};

}