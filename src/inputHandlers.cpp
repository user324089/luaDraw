#include "inputHandlers.hpp"

void mouseMoveHandler::update (bool isPressed, double x, double y) {
    if (wasPressed) {
        if (isPressed) {
            moveX = x - lastX;
            moveY = y - lastY;
        } else {
            wasPressed = false;
            moveX = 0;
            moveY = 0;
        }
    } else {
        wasPressed = isPressed;
        lastX = x;
        lastY = y;
        moveX = 0;
        moveY = 0;
    }
    lastX = x;
    lastY = y;
}

timePassedHandler::timePassedHandler (double _currentTime) : currentTime (_currentTime) {}

void timePassedHandler::update (double _currentTime) {
    deltaTime = _currentTime - currentTime;
    currentTime = _currentTime;
}

double timePassedHandler::getDeltaTime () {
    return deltaTime;
}
