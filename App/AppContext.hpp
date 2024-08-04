#pragma once

#include "Common/Singleton.hpp"
#include "GL/BackgroundContext.hpp"

struct AppContext : public Library::Common::Singleton<AppContext>
{
    Library::GL::BackgroundContext glContext;

    void initializeGLContext();
    void deinitializeGLContext();
};