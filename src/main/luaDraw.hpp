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
#include <memory>

class luaDraw {
    private:
        GLFWwindow * window = nullptr;
        std::unique_ptr <shapeDrawer> mainShapeDrawer = nullptr; // pointer necessary to deffer initialization
        shapeStorage mainShapeStorage;
        colorStorage mainColorStorage;
        callbackStorage mainCallbackStorage;

        configInterpreter mainConfigInterpreter {mainShapeStorage, mainColorStorage, mainCallbackStorage};

        timePassedHandler mainTimePassedHandler {glfwGetTime()};
        mouseMoveHandler mainMouseMoveHandler;

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        static void keyCallback (GLFWwindow* window, int key, int scancode, int action, int mods);
    public:
        luaDraw ();
        bool shouldClose ();
        void update ();
};
