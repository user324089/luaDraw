#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "frequentVerts.hpp"
#include "shapeDrawer.hpp"
#include "shapeStorage.hpp"

void drawShapes (shapeStorage & storage, shapeDrawer & drawer) {

    shapeDrawer::circleDrawingData circleData;
    circleData.pixelRadius = 10;
    for (shapeStorage::circleIterator iter = storage.circleBegin(); iter != storage.circleEnd(); iter++) {
        circleData.x = iter->x;
        circleData.y = iter->y;
        circleData.r = iter->r;
        drawer.drawCircle (circleData);
    }

    shapeDrawer::lineDrawingData lineData;
    lineData.pixelRadius = 10;
    for (shapeStorage::lineIterator iter = storage.lineBegin(); iter != storage.lineEnd(); iter++) {
        lineData.a = iter->a;
        lineData.b = iter->b;
        lineData.c = iter->c;
        drawer.drawLine (lineData);
    }

    shapeDrawer::pointDrawingData pointData;
    pointData.pixelRadius = 12;
    for (shapeStorage::pointIterator iter = storage.pointBegin(); iter != storage.pointEnd(); iter++) {
        pointData.x = iter->x;
        pointData.y = iter->y;
        drawer.drawPoint (pointData);
    }
}

int main () {
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * window = glfwCreateWindow (100, 100, "myApp", nullptr, nullptr);

    glfwMakeContextCurrent (window);

    glewExperimental = GL_TRUE;
    glewInit ();

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shapeDrawer sd;
    shapeStorage ss;

    std::size_t testPointId = ss.newPoint ();
    shapeStorage::point & testPoint = ss.getPoint (testPointId);;
    testPoint.x = 1;
    testPoint.y = 1;

    std::size_t testLineId = ss.newLine ();
    shapeStorage::line & testLine = ss.getLine (testLineId);;
    testLine.a = 1;
    testLine.b = 1;
    testLine.c = -2;

    std::size_t testCircleId = ss.newCircle ();
    shapeStorage::circle & testCircle = ss.getCircle (testCircleId);;
    testCircle.x = 1;
    testCircle.y = 1;
    testCircle.r = 4;

    sd.setPixelsPerUnit (100);

    while (!glfwWindowShouldClose (window)) {

        int width, height;
        glfwGetFramebufferSize (window, &width, &height);
        glViewport (0,0,width, height);
        sd.setFrameDimentions (static_cast<float>(width), static_cast<float>(height));

        glfwPollEvents ();
        glClearColor (0,0,0,1);
        glClear (GL_COLOR_BUFFER_BIT);

        drawShapes (ss, sd);

        glfwSwapBuffers (window);
    }
}
