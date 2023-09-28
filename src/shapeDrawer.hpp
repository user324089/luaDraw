#pragma once

#include <GL/glew.h>
#include "buffer.hpp"
#include "program.hpp"

class shapeDrawer {
    private:

        // these variables MUST remain float for compatibility with uniform
        GLfloat centerX = 0, centerY = 0;
        GLfloat frameWidth = 100, frameHeight = 100;
        GLfloat pixelsPerUnit = 20;

        buffer uniformBuffer;
        constexpr static unsigned int uniformBufferRequiredSize = 32;
        GLuint VAO = 0;

        static const GLchar * const generalVertexShaderSource;

        program pointProgram;
        GLint pointProgramPointUnitLocation = -1;
        GLint pointProgramRadiusPixLocation = -1;
        static const GLchar * const pointFragmentShaderSource;

        program lineProgram;
        GLint lineProgramEquationUnitLocation = -1;
        GLint lineProgramRadiusPixLocation = -1;
        static const GLchar * const lineFragmentShaderSource;

        program circleProgram;
        GLint circleProgramCenterAndRadiusUnitLocation = -1;
        GLint circleProgramLineRadiusPixLocation = -1;
        static const GLchar * const circleFragmentShaderSource;

        void updateUniformBuffer ();
    public:
        shapeDrawer ();
        ~shapeDrawer ();
        void setCenter (float _centerX, float _centerY);
        void moveByPixels (float moveX, float moveY);
        void moveByUnits (float moveX, float moveY);
        void setFrameDimentions (float _frameWidth, float _frameHeight);
        void setPixelsPerUnit (float _pixelsPerUnit);
        struct pointDrawingData {
            float x, y;
            float pixelRadius;
        };
        void drawPoint (const pointDrawingData & data);
        struct lineDrawingData {
            float a, b, c;
            float pixelRadius;
        };
        void drawLine (const lineDrawingData & data);
        struct circleDrawingData {
            float x, y, r;
            float pixelRadius;
        };
        void drawCircle (const circleDrawingData & data);
};

inline constexpr const GLchar * shapeDrawer::generalVertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec2 position;
out vec2 fragPosition;
void main () {
    gl_Position = vec4 (position, 0, 1);
    fragPosition = position;
}
)";

inline constexpr const GLchar * shapeDrawer::pointFragmentShaderSource = R"(
#version 460 core
in vec2 fragPosition;
out vec4 color;

layout (std140, binding=0) uniform drawInfo {
    vec2 centerUnit;
    vec2 frameDimentionsPix;
    float pixelsPerUnit;
};

uniform vec2 pointUnit; // coordinates of the point in units
uniform float radiusPix; // radius of the point in pixels

void main () {
    color = vec4 (fragPosition, 0, 1);
    vec2 centerToPointPix = (pointUnit - centerUnit) * pixelsPerUnit;
    vec2 centerToFragPix = fragPosition/2 * frameDimentionsPix;
    float distancePix = length (centerToPointPix-centerToFragPix);

    if (distancePix > radiusPix+1) {
        discard;
    }

    float alpha = 1 - smoothstep (radiusPix, radiusPix+1, distancePix);

    color = vec4 (1,1,1,alpha);
}
)";

inline constexpr const GLchar * shapeDrawer::lineFragmentShaderSource = R"(
#version 460 core
in vec2 fragPosition;
out vec4 color;

layout (std140, binding=0) uniform drawInfo {
    vec2 centerUnit;
    vec2 frameDimentionsPix;
    float pixelsPerUnit;
};

uniform vec3 equationUnit; // coordinates of the point in units
uniform float radiusPix; // radius of the point in pixels

float calculateDistanceUnit (vec2 fragmentUnit) {
    float signedNumerator = dot (equationUnit, vec3 (fragmentUnit, 1));
    float numerator = abs (signedNumerator);
    float denominator = length (equationUnit.xy);
    return numerator / denominator;
}

void main () {
    vec2 centerToFragPix = fragPosition/2 * frameDimentionsPix;
    vec2 centerToFragUnit = centerToFragPix / pixelsPerUnit;
    vec2 fragmentUnit = centerUnit + centerToFragUnit;

    float distanceUnit = calculateDistanceUnit (fragmentUnit);
    float distancePix = distanceUnit * pixelsPerUnit;

    if (distancePix > radiusPix+1) {
        discard;
    }

    float alpha = 1 - smoothstep (radiusPix, radiusPix+1, distancePix);

    color = vec4 (1,1,1,alpha);
}
)";

inline constexpr const GLchar * shapeDrawer::circleFragmentShaderSource = R"(
#version 460 core
in vec2 fragPosition;
out vec4 color;

layout (std140, binding=0) uniform drawInfo {
    vec2 centerUnit;
    vec2 frameDimentionsPix;
    float pixelsPerUnit;
};

uniform vec3 circleCenterAndRadiusUnit; // coordinates of the point in units
uniform float lineRadiusPix; // radius of the point in pixels

void main () {
    vec2 centerToFragPix = fragPosition/2 * frameDimentionsPix;
    vec2 centerToFragUnit = centerToFragPix / pixelsPerUnit;
    vec2 fragmentUnit = centerUnit + centerToFragUnit;

    float circleCenterToFragmentUnit = length(fragmentUnit - circleCenterAndRadiusUnit.xy);
    float distanceFromLineUnit = abs (circleCenterToFragmentUnit - circleCenterAndRadiusUnit.z);
    float distanceFromLinePix = distanceFromLineUnit * pixelsPerUnit;

    if (distanceFromLinePix > lineRadiusPix+1) {
        discard;
    }

    float alpha = 1 - smoothstep (lineRadiusPix, lineRadiusPix+1, distanceFromLinePix);

    color = vec4 (1,1,1,alpha);
}
)";
