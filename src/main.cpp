#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "frequentVerts.hpp"
#include "shapeDrawer.hpp"
#include "shapeStorage.hpp"
#include "configInterpreter.hpp"
#include "inputHandlers.hpp"
#include <cmath>

constexpr static float arrowMoveSpeed = 500;
constexpr static float scrollEnlargeSpeed = 1.3f;

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

struct windowData {
    double scrolledX = 0, scrolledY = 0;
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    windowData * data = static_cast<windowData*>(glfwGetWindowUserPointer (window));
    data->scrolledX += xoffset;
    data->scrolledY += yoffset;
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

    windowData mainWindowData;
    glfwSetWindowUserPointer (window, &mainWindowData);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shapeDrawer sd;
    //shapeStorage ss;

    shapeStorage mainShapeStorage;

    glfwSetScrollCallback(window, scroll_callback);

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

function Update ()
    c1.x = luaDraw.getTime();
    c1.r = luaDraw.getTime();
    pt3 = luaDraw.newPoint ();
    pt3.x = c1.x;
    pt3.y = c1.y;
end
)";


    configInterpreter mainConfigInterpreter (mainShapeStorage);
    mainConfigInterpreter.setupFromString (luaProgram);

    sd.setPixelsPerUnit (100);

    mouseMoveHandler mainMouseMoveHandler;

    timePassedHandler mainTimePassedHandler (glfwGetTime ());

    while (!glfwWindowShouldClose (window)) {

        mainTimePassedHandler.update (glfwGetTime());

        bool isMousePressed = (glfwGetMouseButton (window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
        double mouseX, mouseY;
        glfwGetCursorPos (window, &mouseX, &mouseY);

        mainMouseMoveHandler.update (isMousePressed, mouseX, mouseY);

        float moveByPixelsX = -static_cast<float>(mainMouseMoveHandler.getMoveX());
        float moveByPixelsY = static_cast<float>(mainMouseMoveHandler.getMoveY());

        if (glfwGetKey (window, GLFW_KEY_UP) == GLFW_PRESS) {
            moveByPixelsY += arrowMoveSpeed * static_cast<float>(mainTimePassedHandler.getDeltaTime());
        }
        if (glfwGetKey (window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            moveByPixelsY -= arrowMoveSpeed * static_cast<float>(mainTimePassedHandler.getDeltaTime());
        }
        if (glfwGetKey (window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            moveByPixelsX -= arrowMoveSpeed * static_cast<float>(mainTimePassedHandler.getDeltaTime());
        }
        if (glfwGetKey (window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            moveByPixelsX += arrowMoveSpeed * static_cast<float>(mainTimePassedHandler.getDeltaTime());
        }

        sd.moveByPixels (moveByPixelsX, moveByPixelsY);

        sd.enlargeView (static_cast<float>(std::pow(scrollEnlargeSpeed, mainWindowData.scrolledY)));
        mainWindowData.scrolledY = 0;

        int width, height;
        glfwGetFramebufferSize (window, &width, &height);
        glViewport (0,0,width, height);
        sd.setFrameDimentions (static_cast<float>(width), static_cast<float>(height));

        mainConfigInterpreter.update ();

        glfwPollEvents ();
        glClearColor (0,0,0,1);
        glClear (GL_COLOR_BUFFER_BIT);

        //drawShapes (ss, sd);
        drawShapes (mainShapeStorage, sd);

        glfwSwapBuffers (window);
    }
}
