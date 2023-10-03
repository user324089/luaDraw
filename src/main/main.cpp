#include "luaDraw.hpp"

int main () {
    luaDraw mainLuaDraw;
    while (!mainLuaDraw.shouldClose()) {
        mainLuaDraw.update();
    }
}
