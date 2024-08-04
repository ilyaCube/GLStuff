#include "AppLauncher.hpp"

int main(int argc, char** argv)
{
    auto launcher = App::AppLauncher::create();
    return launcher != nullptr ? launcher->start(argc, argv) : 1;
}