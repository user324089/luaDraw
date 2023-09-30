extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int testFunc (lua_State * L) {
    lua_pushnumber (L, 1.2);
    return 1;
}

luaL_Reg constructionFunctions [] = {
    {"testFunc", testFunc},
    {NULL, NULL}
};

extern "C" int luaopen_constructions (lua_State * L) {
    luaL_newlib(L, constructionFunctions);
    return 1;
}
