#include "shapeDrawer.hpp"
#include "frequentVerts.hpp"
#include <cstring>

void shapeDrawer::updateUniformBuffer () {
    char bufferSubData [uniformBufferRequiredSize];
    std::memcpy (bufferSubData, &centerX, sizeof(GLfloat));
    std::memcpy (bufferSubData + 4, &centerY, sizeof(GLfloat));
    std::memcpy (bufferSubData + 8, &frameWidth, sizeof(GLfloat));
    std::memcpy (bufferSubData + 12, &frameHeight, sizeof(GLfloat));
    std::memcpy (bufferSubData + 16, &pixelsPerUnit, sizeof(GLfloat));
    uniformBuffer.subdata (0, uniformBufferRequiredSize, bufferSubData);
}

shapeDrawer::shapeDrawer () {
    glCreateVertexArrays (1, &VAO);
    buffer vertexPositionBuffer;
    vertexPositionBuffer.data (sizeof (frequentVerts::squareLocations), frequentVerts::squareLocations, GL_STATIC_DRAW);
    glBindVertexArray (VAO); {

        vertexPositionBuffer.bind (GL_ARRAY_BUFFER);
        glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (frequentVerts::vert2d), reinterpret_cast<void*>(offsetof (frequentVerts::vert2d, coords)));
        glEnableVertexAttribArray (0);

    } glBindVertexArray (0);

    shader generalVertexShader (GL_VERTEX_SHADER);
    generalVertexShader.make (1, &generalVertexShaderSource);

    shader pointFragmentShader (GL_FRAGMENT_SHADER);
    pointFragmentShader.make (1, &pointFragmentShaderSource);

    shader lineFragmentShader (GL_FRAGMENT_SHADER);
    lineFragmentShader.make (1, &lineFragmentShaderSource);

    pointProgram.make (generalVertexShader, pointFragmentShader);
    lineProgram.make (generalVertexShader, lineFragmentShader);

    uniformBuffer.data (uniformBufferRequiredSize, nullptr, GL_STATIC_DRAW);

    pointProgramPointUnitLocation = pointProgram.getUniformLocation ("pointUnit");
    pointProgramRadiusPixLocation = pointProgram.getUniformLocation ("radiusPix");

    lineProgramEquationUnitLocation = lineProgram.getUniformLocation ("equationUnit");
    lineProgramRadiusPixLocation = lineProgram.getUniformLocation ("radiusPix");
}

shapeDrawer::~shapeDrawer () {
    glDeleteVertexArrays (1, &VAO);
}

void shapeDrawer::setCenter (float _centerX, float _centerY) {
    centerX = _centerX;
    centerY = _centerY;
}

void shapeDrawer::setFrameDimentions (float _frameWidth, float _frameHeight) {
    frameWidth = _frameWidth;
    frameHeight = _frameHeight;
}

void shapeDrawer::setPixelsPerUnit (float _pixelsPerUnit) {
    pixelsPerUnit = _pixelsPerUnit;
}

void shapeDrawer::drawPoint (const pointDrawingData & data) {
    updateUniformBuffer ();
    uniformBuffer.bindRange (GL_UNIFORM_BUFFER, 0, 0, uniformBufferRequiredSize);
    pointProgram.use ();
    glUniform2f (pointProgramPointUnitLocation, data.x, data.y);
    glUniform1f (pointProgramRadiusPixLocation, data.pixelRadius);
    glBindVertexArray (VAO);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void shapeDrawer::drawLine (const lineDrawingData & data) {
    updateUniformBuffer ();
    uniformBuffer.bindRange (GL_UNIFORM_BUFFER, 0, 0, uniformBufferRequiredSize);
    lineProgram.use ();
    glUniform3f (lineProgramEquationUnitLocation, data.a, data.b, data.c);
    glUniform1f (lineProgramRadiusPixLocation, data.pixelRadius);
    glBindVertexArray (VAO);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}
