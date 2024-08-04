#include "AppContext.hpp"

void AppContext::initializeGLContext()
{
    glContext = Library::GL::createContext();
}

void AppContext::deinitializeGLContext()
{
    glContext = nullptr;
}