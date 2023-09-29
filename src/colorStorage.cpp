#include "colorStorage.hpp"
#include <stdexcept>

colorStorage::color::color () {
    uniformColorBuffer.data (16, colorComponents, GL_STATIC_DRAW);
}

void colorStorage::color::updateUniform () {
    uniformColorBuffer.subdata (0, 16, colorComponents);
}

void colorStorage::color::setComponents (const float * _colorComponents) {
    colorComponents[0] = _colorComponents[0];
    colorComponents[1] = _colorComponents[1];
    colorComponents[2] = _colorComponents[2];
    colorComponents[3] = _colorComponents[3];
    updateUniform ();
}

void colorStorage::color::bindToUnit (GLuint unit) {
    uniformColorBuffer.bindRange (GL_UNIFORM_BUFFER, unit, 0, 16);
}

int colorStorage::newColor (float r, float g, float b, float a) {
    std::size_t id = availableColors.size();
    availableColors.emplace_back ();
    color & newColor = availableColors.back();
    newColor.colorComponents[0] = r;
    newColor.colorComponents[1] = g;
    newColor.colorComponents[2] = b;
    newColor.colorComponents[3] = a;
    newColor.updateUniform();
    return static_cast<int>(id);
}

colorStorage::color & colorStorage::getColor (int id) {
    if (id < 0) {
        throw std::runtime_error ("cannot access color with negative id");
    }
    return availableColors [static_cast<std::size_t>(id)];
}
