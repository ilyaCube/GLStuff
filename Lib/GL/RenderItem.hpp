#pragma once

namespace Library::GL {

class RenderItem
{
    struct Attribute
    {
        int id;
        int count;
        int glType;
        int offset;
        bool isNormalize;
    };
};

}