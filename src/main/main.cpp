#include "luaDraw.hpp"
#include <iostream>
#include <map>


int main () {
    luaDraw mainLuaDraw;
    while (!mainLuaDraw.shouldClose()) {
        mainLuaDraw.update();
    }
}
