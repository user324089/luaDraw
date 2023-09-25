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

        void updateUniformBuffer ();
    public:
        shapeDrawer ();
        ~shapeDrawer ();
        void setCenter (float _centerX, float _centerY);
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
    if (distancePix < radiusPix) {
        color = vec4 (0,0,1,1);
    } else {
        color = vec4 (0,1,0,1);
    }
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

    if (distancePix < radiusPix) {
        color = vec4 (0,0,1,1);
    } else {
        color = vec4 (0,1,0,1);
    }
}
)";
