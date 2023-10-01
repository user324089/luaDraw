extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <cmath>

constexpr static double samePointEpsilon = 1e-7;

struct point {
    double x, y;
};

struct line {
    double a, b, c;
};

struct circle {
    double x, y, r;
};

inline double square (double d) {
    return d * d;
}

static double distance (const point & pt1, const point & pt2) {
    return std::sqrt (square(pt1.x-pt2.x) + square (pt1.y - pt2.y));
}

#define GET_NUMBER_FIELD(L, index, object, field) \
    lua_getfield ((L), (index), (#field)); \
    object.field = lua_tonumber (L, -1); \
    lua_pop (L, 1);

static point loadPoint (lua_State * L, int index) {
    point pt;
    GET_NUMBER_FIELD (L, index, pt, x);
    GET_NUMBER_FIELD (L, index, pt, y);
    return pt;
}

static line loadLine (lua_State * L, int index) {
    line lin;
    GET_NUMBER_FIELD (L, index, lin, a);
    GET_NUMBER_FIELD (L, index, lin, b);
    GET_NUMBER_FIELD (L, index, lin, c);
    return lin;
}

static circle loadCircle (lua_State * L, int index) {
    circle circ;
    GET_NUMBER_FIELD (L, index, circ, x);
    GET_NUMBER_FIELD (L, index, circ, y);
    GET_NUMBER_FIELD (L, index, circ, r);
    return circ;
}

#undef GET_NUMBER_FIELD

static int lineThroughTwoPoints (lua_State * L) {
    luaL_checkudata (L, 1, "luaDraw.point");
    luaL_checkudata (L, 2, "luaDraw.point");
    point pt1 = loadPoint (L, 1);
    point pt2 = loadPoint (L, 2);
    if (distance (pt1, pt2) < samePointEpsilon) {
        lua_pushnil (L);
        return 1;
    }

    lua_getfield (L, LUA_REGISTRYINDEX, "luaDraw");
    lua_getfield (L, -1, "newLine");
    lua_call (L, 0, 1);

    lua_pushnumber (L, pt1.y - pt2.y);
    lua_setfield (L, -2, "a");

    lua_pushnumber (L, pt2.x - pt1.x);
    lua_setfield (L, -2, "b");

    lua_pushnumber (L, pt1.x * pt2.y - pt2.x * pt1.y);
    lua_setfield (L, -2, "c");

    lua_insert (L, -2);
    lua_pop (L, 1);

    return 1;
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
    {"lineThroughTwoPoints", lineThroughTwoPoints},
    {"testFunc", testFunc},
    {NULL, NULL}
};

extern "C" int luaopen_constructions (lua_State * L) {
    luaL_newlib(L, constructionFunctions);
    return 1;
}
