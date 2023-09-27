#pragma once

#include "shapeStorage.hpp"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class configInterpreter {
    private:
        lua_State * L;
        shapeStorage & configuredShapeStorage;

        static int newPoint (lua_State * L);
        static int newLine (lua_State * L);
        static int newCircle (lua_State * L);

        static int getPointField (lua_State * L);
        static int getLineField (lua_State * L);
        static int getCircleField (lua_State * L);

        static int setPointField (lua_State * L);
        static int setLineField (lua_State * L);
        static int setCircleField (lua_State * L);

        static const luaL_Reg luaDrawModuleFunctions [];
        static const luaL_Reg pointMetaTableFunctions [];
        static const luaL_Reg lineMetaTableFunctions [];
        static const luaL_Reg circleMetaTableFunctions [];
    public:
        configInterpreter (shapeStorage & _configuredShapeStorage);
        void setupFromString (const std::string luaText);
        void setupFromFile (const std::string filename);
        ~configInterpreter ();

        void update ();
};

inline constexpr luaL_Reg configInterpreter::luaDrawModuleFunctions [] = {
    {"newPoint", newPoint},
    {"newLine", newLine},
    {"newCircle", newCircle},
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
