#pragma once

#include "GL.hpp"

namespace Library::GL {

template<typename T>
struct GLType;

template<>
struct GLType<GLbyte>
{
    constexpr static int value = GL_BYTE;
};

template<>
struct GLType<GLubyte>
{
    constexpr static int value = GL_UNSIGNED_BYTE;
};

template<>
struct GLType<GLshort>
{
    constexpr static int value = GL_SHORT;
};

template<>
struct GLType<GLushort>
{
    constexpr static int value = GL_UNSIGNED_SHORT;
};

template<>
struct GLType<GLint>
{
    constexpr static int value = GL_INT;
};

template<>
struct GLType<GLuint>
{
    constexpr static int value = GL_UNSIGNED_INT;
};

template<>
struct GLType<GLfloat>
{
    constexpr static int value = GL_FLOAT;
};

template<>
struct GLType<GLdouble>
{
    constexpr static int value = GL_DOUBLE;
};

inline int glBool(bool value)
{
    return value ? GL_TRUE : GL_FALSE;
}

}