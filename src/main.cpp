#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "frequentVerts.hpp"
#include "shapeDrawer.hpp"

int main () {
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * window = glfwCreateWindow (100, 100, "myApp", nullptr, nullptr);

    glfwMakeContextCurrent (window);

    glewExperimental = GL_TRUE;
    glewInit ();

    shapeDrawer sd;

    shapeDrawer::pointDrawingData data;
    data.x = 1;
    data.y = 1;
    data.pixelRadius = 10;

    sd.setPixelsPerUnit (100);

    while (!glfwWindowShouldClose (window)) {

        int width, height;
        glfwGetFramebufferSize (window, &width, &height);
        glViewport (0,0,width, height);
        sd.setFrameDimentions (static_cast<float>(width), static_cast<float>(height));

        glfwPollEvents ();
        glClearColor (1,0,0,1);
        glClear (GL_COLOR_BUFFER_BIT);
        sd.drawPoint (data);
        glfwSwapBuffers (window);
    }
}
