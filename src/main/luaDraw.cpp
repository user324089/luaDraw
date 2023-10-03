#include "luaDraw.hpp"
#include <cmath>

extern const float arrowMoveSpeed;
extern const float scrollEnlargeSpeed;

void luaDraw::scroll_callback(GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset) {
    luaDraw * self = static_cast<luaDraw*>(glfwGetWindowUserPointer (window));
    self->mainShapeDrawer->enlargeView (static_cast<float>(std::pow(scrollEnlargeSpeed, yoffset)));
}

luaDraw::luaDraw () {
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow (100, 100, "myApp", nullptr, nullptr);

    glfwMakeContextCurrent (window);

    glewExperimental = GL_TRUE;
    glewInit ();

    glfwSetWindowUserPointer (window, this);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetScrollCallback(window, scroll_callback);

    mainConfigInterpreter.initializeDefaultColors ();
    mainConfigInterpreter.setupFromFile ("script.lua");

    mainShapeDrawer = std::make_unique<shapeDrawer> ();
    mainShapeDrawer->setPixelsPerUnit (100);
}

bool luaDraw::shouldClose () {
    return glfwWindowShouldClose (window);
}

void luaDraw::update () {
    glfwPollEvents ();
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

    mainShapeDrawer->moveByPixels (moveByPixelsX, moveByPixelsY);

    int width, height;
    glfwGetFramebufferSize (window, &width, &height);
    glViewport (0,0,width, height);
    mainShapeDrawer->setFrameDimentions (static_cast<float>(width), static_cast<float>(height));

    mainConfigInterpreter.update ();
    glClearColor (0,0,0,1);
    glClear (GL_COLOR_BUFFER_BIT);

    mainShapeDrawer->drawShapes (mainShapeStorage, mainColorStorage);

    glfwSwapBuffers (window);
}
