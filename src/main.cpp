#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.hpp"
#include "program.hpp"
#include "texture.hpp"

int main () {
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * window = glfwCreateWindow (100, 100, "myApp", nullptr, nullptr);
    glewExperimental = GL_TRUE;
    glewInit ();

    while (!glfwWindowShouldClose (window)) {
        glfwPollEvents ();
        glClearColor (1,0,0,1);
        glClear (GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers (window);
    }
}
