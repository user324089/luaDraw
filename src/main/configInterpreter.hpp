#pragma once

#include "shapeStorage.hpp"
#include "colorStorage.hpp"
#include "callbackStorage.hpp"
#include <chrono>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class configInterpreter {
    private:
        lua_State * L;
        shapeStorage & configuredShapeStorage;
        shapeStorage::sizeMemento configuredShapeStorageMementoAfterSetup;

        colorStorage & configuredColorStorage;

        callbackStorage & configuredCallbackStorage;

        static int newPoint (lua_State * L);
        static int newLine (lua_State * L);
        static int newCircle (lua_State * L);

        static int getPointField (lua_State * L);
        static int getLineField (lua_State * L);
        static int getCircleField (lua_State * L);

        static int setPointField (lua_State * L);
        static int setLineField (lua_State * L);
        static int setCircleField (lua_State * L);

        static int getTime (lua_State * L);

        static int setCallback (lua_State * L);

        static int newColor (lua_State * L);

        static int setColorField (lua_State * L);
        static int getColorField (lua_State * L);

        static const luaL_Reg luaDrawModuleFunctions [];
        static const luaL_Reg pointMetaTableFunctions [];
        static const luaL_Reg lineMetaTableFunctions [];
        static const luaL_Reg circleMetaTableFunctions [];

        static const luaL_Reg colorMetaTableFunctions [];

        void runUpdateFunction ();

        std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();

    public:
        configInterpreter (shapeStorage & _configuredShapeStorage, colorStorage & _configuredColorStorage,
                callbackStorage & _configuredCallbackStorage);
        void initializeDefaultColors ();
        void setupFromString (const std::string luaText);
        void setupFromFile (const std::string filename);
        void update ();
        ~configInterpreter ();

};

inline constexpr luaL_Reg configInterpreter::luaDrawModuleFunctions [] = {
    {"newPoint", newPoint},
    {"newLine", newLine},
    {"newCircle", newCircle},
    {"getTime", getTime},
    {"setCallback", setCallback},
    {NULL, NULL}
};

inline constexpr luaL_Reg configInterpreter::pointMetaTableFunctions [] = {
    {"__index", getPointField},
    {"__newindex", setPointField},
    {NULL, NULL}
};

inline constexpr luaL_Reg configInterpreter::lineMetaTableFunctions [] = {
    {"__index", getLineField},
    {"__newindex", setLineField},
    {NULL, NULL}
};

inline constexpr luaL_Reg configInterpreter::circleMetaTableFunctions [] = {
    {"__index", getCircleField},
    {"__newindex", setCircleField},
    {NULL, NULL}
};

inline constexpr luaL_Reg configInterpreter::colorMetaTableFunctions [] = {
    {"__index", getColorField},
    {"__newindex", setColorField},
    {NULL, NULL}
};
