extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int testFunc (lua_State * L) {

    lua_getfield (L, LUA_REGISTRYINDEX, "luaDraw");

    lua_getfield (L, -1, "newCircle");
    lua_call (L, 0, 1);

    lua_pushnumber (L, -1);
    lua_setfield (L, -2, "x");

    lua_pushnumber (L, -2);
    lua_setfield (L, -2, "y");

    lua_pushnumber (L, 1);
    lua_setfield (L, -2, "r");

    lua_insert (L, -2);
    lua_pop (L, 1);

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
