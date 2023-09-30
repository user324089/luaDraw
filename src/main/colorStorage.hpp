#pragma once

#include "buffer.hpp"
#include <vector>

class colorStorage {
    public:
        struct color {
            buffer uniformColorBuffer;
            float colorComponents [4] = {0,0,0,0};
            int luaIndex = -1;
            color ();
            void updateUniform ();
            void setComponents (const float * _colorComponents);
            void bindToUnit (GLuint unit);
        };
    private:
        std::vector<color> availableColors;
    public:
        int newColor (float r = 0, float g = 0, float b = 0, float a = 1);
        color & getColor (int id);
};
