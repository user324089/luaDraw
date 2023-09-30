#pragma once

class mouseMoveHandler {
    private:
        bool wasPressed = false;
        double lastX = 0, lastY = 0;
        double moveX = 0, moveY = 0;
    public:
        void update (bool isPressed, double x, double y);
        double getMoveX () { return moveX; }
        double getMoveY () { return moveY; }
};


class timePassedHandler {
    private:
        double currentTime;
        double deltaTime = 0;
    public:
        timePassedHandler (double _currentTime);
        void update (double _currentTime);
        double getDeltaTime ();
};
