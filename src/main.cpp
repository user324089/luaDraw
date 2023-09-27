#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "frequentVerts.hpp"
#include "shapeDrawer.hpp"
#include "shapeStorage.hpp"
#include "configInterpreter.hpp"


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
    //shapeStorage ss;

    shapeStorage mainShapeStorage;


    const char * luaProgram = R"(
pt = luaDraw.newPoint();
print('printing pt.x', pt.x)
pt.x = 1.3;
print('printing pt.x', pt.x)

pt2 = luaDraw.newPoint();
print('printing pt2.x', pt2.x)
pt2.x = -3.2;
print('printing pt2.x', pt2.x)

l1 = luaDraw.newLine();
l1.a = 1
l1.b = 0.1
l1.c = 0

c1 = luaDraw.newCircle()
c1.x = 1
c1.y = 1
c1.r = 4
)";


    configInterpreter mainConfigInterpreter (mainShapeStorage);
    mainConfigInterpreter.setupFromString (luaProgram);

    sd.setPixelsPerUnit (100);

    while (!glfwWindowShouldClose (window)) {

        int width, height;
        glfwGetFramebufferSize (window, &width, &height);
        glViewport (0,0,width, height);
        sd.setFrameDimentions (static_cast<float>(width), static_cast<float>(height));

        glfwPollEvents ();
        glClearColor (0,0,0,1);
        glClear (GL_COLOR_BUFFER_BIT);

        //drawShapes (ss, sd);
        drawShapes (mainShapeStorage, sd);

        glfwSwapBuffers (window);
    }
}
