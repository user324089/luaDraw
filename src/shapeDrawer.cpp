#include "shapeDrawer.hpp"
#include "frequentVerts.hpp"
#include <cstring>
#include <algorithm>
#include <functional>

extern const float totalPointRadius;
extern const float totalLineRadius;
extern const float totalCircleLineRadius;
extern const float borderRadius;

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

    shader circleFragmentShader (GL_FRAGMENT_SHADER);
    circleFragmentShader.make (1, &circleFragmentShaderSource);

    pointProgram.make (generalVertexShader, pointFragmentShader);
    lineProgram.make (generalVertexShader, lineFragmentShader);
    circleProgram.make (generalVertexShader, circleFragmentShader);

    uniformBuffer.data (uniformBufferRequiredSize, nullptr, GL_STATIC_DRAW);

    pointProgramPointUnitLocation = pointProgram.getUniformLocation ("pointUnit");
    pointProgramRadiusPixLocation = pointProgram.getUniformLocation ("radiusPix");

    lineProgramEquationUnitLocation = lineProgram.getUniformLocation ("equationUnit");
    lineProgramRadiusPixLocation = lineProgram.getUniformLocation ("radiusPix");

    circleProgramCenterAndRadiusUnitLocation = circleProgram.getUniformLocation ("circleCenterAndRadiusUnit");
    circleProgramLineRadiusPixLocation = circleProgram.getUniformLocation ("lineRadiusPix");

    defaultColor.setComponents (defaultColorData);
    borderColor.setComponents (borderColorData);

}

shapeDrawer::~shapeDrawer () {
    glDeleteVertexArrays (1, &VAO);
}

void shapeDrawer::setCenter (float _centerX, float _centerY) {
    centerX = _centerX;
    centerY = _centerY;
}

void shapeDrawer::moveByPixels (float moveX, float moveY) {
    float moveXUnit = moveX / pixelsPerUnit;
    float moveYUnit = moveY / pixelsPerUnit;
    centerX += moveXUnit;
    centerY += moveYUnit;
}

void shapeDrawer::moveByUnits (float moveX, float moveY) {
    centerX += moveX;
    centerY += moveY;
}

void shapeDrawer::setFrameDimentions (float _frameWidth, float _frameHeight) {
    frameWidth = _frameWidth;
    frameHeight = _frameHeight;
}

void shapeDrawer::setPixelsPerUnit (float _pixelsPerUnit) {
    pixelsPerUnit = _pixelsPerUnit;
}

void shapeDrawer::enlargeView (float times) {
    pixelsPerUnit *= times;
}

void shapeDrawer::drawPoint (const shapeStorage::point & pt, float pixRadius) {
    updateUniformBuffer ();
    uniformBuffer.bindRange (GL_UNIFORM_BUFFER, 0, 0, uniformBufferRequiredSize);
    pointProgram.use ();
    glUniform2f (pointProgramPointUnitLocation, pt.x, pt.y);
    glUniform1f (pointProgramRadiusPixLocation, pixRadius);
    glBindVertexArray (VAO);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void shapeDrawer::drawLine (const shapeStorage::line & lin, float pixRadius) {
    updateUniformBuffer ();
    uniformBuffer.bindRange (GL_UNIFORM_BUFFER, 0, 0, uniformBufferRequiredSize);
    lineProgram.use ();
    glUniform3f (lineProgramEquationUnitLocation, lin.a, lin.b, lin.c);
    glUniform1f (lineProgramRadiusPixLocation, pixRadius);
    glBindVertexArray (VAO);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void shapeDrawer::drawCircle (const shapeStorage::circle & circ, float pixRadius) {
    updateUniformBuffer ();
    uniformBuffer.bindRange (GL_UNIFORM_BUFFER, 0, 0, uniformBufferRequiredSize);
    circleProgram.use ();
    glUniform3f (circleProgramCenterAndRadiusUnitLocation, circ.x, circ.y, circ.r);
    glUniform1f (circleProgramLineRadiusPixLocation, pixRadius);
    glBindVertexArray (VAO);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

template <typename SHAPE_TYPE>
void shapeDrawer::drawOneShape (const SHAPE_TYPE & shape, void (shapeDrawer::* drawFunc) (const SHAPE_TYPE&, float), colorStorage & usedColorStorage, float totalRadius) {
    borderColor.bindToUnit (1);
    (this->*drawFunc) (shape, totalRadius);

    if (shape.color == -1) {
        defaultColor.bindToUnit (1);
    } else {
        usedColorStorage.getColor (shape.color).bindToUnit (1);
    }
    (this->*drawFunc) (shape, totalRadius - borderRadius);
}

void shapeDrawer::drawShapes (shapeStorage & drawnShapesStorage, colorStorage & usedColorStorage) {

    std::for_each (drawnShapesStorage.circleBegin(), drawnShapesStorage.circleEnd(), [&] (auto& circ) {
            drawOneShape (circ, &shapeDrawer::drawCircle, usedColorStorage, totalCircleLineRadius);
            });

    std::for_each (drawnShapesStorage.lineBegin(), drawnShapesStorage.lineEnd(), [&] (auto& lin) {
            drawOneShape (lin, &shapeDrawer::drawLine, usedColorStorage, totalLineRadius);
            });

    std::for_each (drawnShapesStorage.pointBegin(), drawnShapesStorage.pointEnd(), [&] (auto& pt) {
            drawOneShape (pt, &shapeDrawer::drawPoint, usedColorStorage, totalPointRadius);
            });
}
