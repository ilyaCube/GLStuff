#pragma once

#include <QDebug>
#include <QOpenGLFunctions_3_0>

namespace Library::GL {

#define GL_CHECK_ERROR(glErr)                                                   \
    if(glErr != GL_NO_ERROR) {                                                  \
        auto errorMsg = Library::GL::glErrorString(glErr);                      \
        qCritical("OpenGL error: %s | %s | %i", errorMsg, __FILE__, __LINE__);  \
    }

#define GL_CHECK() GL_CHECK_ERROR(Library::GL::functions()->glGetError())

inline QOpenGLFunctions_3_0 *functions()
{
    auto context = QOpenGLContext::currentContext();
    if (context == nullptr) {
        qFatal("OpenGL context is null");
    }
    auto functions = context->versionFunctions<QOpenGLFunctions_3_0>();
    if (functions == nullptr) {
        qFatal("Unsupported OpenGL version");
    }
    return functions;
}

char const* glErrorString(GLenum const err) noexcept;

}
