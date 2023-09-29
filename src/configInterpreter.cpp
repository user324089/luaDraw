#include "configInterpreter.hpp"
#include <stdexcept>
#include <string>

struct startingAvailableColor {
    const char * name;
    const float data [4];
};

constexpr static startingAvailableColor allStartingAvailableColors [] = {
    {"red", {1,0,0,1}},
    {"green", {0,1,0,1}},
    {"blue", {0,0,1,1}},
    {"cyan", {0,1,1,1}},
    {"magenta", {1,0,1,1}},
    {"yellow", {1,1,0,1}},
    {"black", {0,0,0,1}},
    {"white", {1,1,1,1}},
    {"brown", {0.64f,0.16f,0.16f,1}},
};

constexpr static float defaultNewColor [] = {0,0,0,1};
static void luaPushNewColor (lua_State * L, colorStorage & configuredColorStorage, const float * colorData = defaultNewColor) {
    int newId = configuredColorStorage.newColor ();
    int * idPtr = static_cast<int*>(lua_newuserdata(L, sizeof(size_t)));
    *idPtr = newId;
    luaL_setmetatable (L, "luaDraw.color");

    lua_pushnumber (L, static_cast<double>(colorData[0]));
    lua_setfield (L, -2, "r");
    lua_pushnumber (L, static_cast<double>(colorData[1]));
    lua_setfield (L, -2, "g");
    lua_pushnumber (L, static_cast<double>(colorData[2]));
    lua_setfield (L, -2, "b");
    lua_pushnumber (L, static_cast<double>(colorData[3]));
    lua_setfield (L, -2, "a");

    lua_pushvalue (L, -1);
    configuredColorStorage.getColor (newId).luaIndex = luaL_ref (L, LUA_REGISTRYINDEX);
}

configInterpreter::configInterpreter (shapeStorage & _configuredShapeStorage, colorStorage & _configuredColorStorage)
    : configuredShapeStorage (_configuredShapeStorage), configuredColorStorage (_configuredColorStorage){

    L = luaL_newstate();
    luaL_openlibs (L);

    luaL_newmetatable (L, "luaDraw.color");
    lua_pushlightuserdata (L, this);
    luaL_setfuncs (L, colorMetaTableFunctions, 1);
    lua_pop (L, 1);

    luaL_newmetatable (L, "luaDraw.point");
    lua_pushlightuserdata (L, this);
    luaL_setfuncs (L, pointMetaTableFunctions, 1);
    lua_pop (L, 1);

    luaL_newmetatable (L, "luaDraw.line");
    lua_pushlightuserdata (L, this);
    luaL_setfuncs (L, lineMetaTableFunctions, 1);
    lua_pop (L, 1);

    luaL_newmetatable (L, "luaDraw.circle");
    lua_pushlightuserdata (L, this);
    luaL_setfuncs (L, circleMetaTableFunctions, 1);
    lua_pop (L, 1);

    luaL_newlibtable (L, luaDrawModuleFunctions);

    lua_pushlightuserdata (L, this);
    luaL_setfuncs (L, luaDrawModuleFunctions, 1);

    int numStartingAvailableColors = sizeof(allStartingAvailableColors)/sizeof(startingAvailableColor);
    lua_createtable (L, 0, numStartingAvailableColors); // colors table
    lua_pushlightuserdata (L, this);
    lua_pushcclosure (L, newColor, 1);
    lua_setfield (L, -2, "newColor");

    for (int i = 0; i < numStartingAvailableColors; i++) {
        luaPushNewColor (L, configuredColorStorage, allStartingAvailableColors[i].data);
        lua_setfield (L, -2, allStartingAvailableColors[i].name);
    }

    lua_setfield (L, -2, "colors");

    lua_setglobal (L, "luaDraw");

}

void configInterpreter::runUpdateFunction () {
    lua_getglobal (L, "Update");
    if (lua_isnil(L, -1)) {
        lua_pop (L, 1);
        return;
    }
    if (!lua_isfunction(L, -1)) {
        throw std::runtime_error ("Update is not a function");
    }
    if (lua_pcall(L, 0, 0, 0)) {
        throw std::runtime_error (std::string ("Error calling update: ") + lua_tostring(L, -1));
    }
}

void configInterpreter::setupFromString (const std::string luaText) {
    if (luaL_loadstring (L, luaText.c_str()) || lua_pcall (L, 0, 0, 0)) {
        throw std::runtime_error(std::string ("config reading error: ") + lua_tostring(L, -1));
    }
    configuredShapeStorageMementoAfterSetup = configuredShapeStorage.exportSize();
    runUpdateFunction ();
}

void configInterpreter::setupFromFile (const std::string filename) {
    if (luaL_loadfile (L, filename.c_str()) || lua_pcall (L, 0, 0, 0)) {
        throw std::runtime_error(std::string ("config reading error: ") + lua_tostring(L, -1));
    }
    configuredShapeStorageMementoAfterSetup = configuredShapeStorage.exportSize();
    runUpdateFunction ();
}

void configInterpreter::update () {
    configuredShapeStorage.importSize (configuredShapeStorageMementoAfterSetup);
    runUpdateFunction ();
}

configInterpreter::~configInterpreter () {
    lua_close (L);
}

int configInterpreter::newPoint (lua_State * L) {
    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    std::size_t newId = self->configuredShapeStorage.newPoint();
    std::size_t * idPtr = static_cast<size_t*>(lua_newuserdata(L, sizeof(size_t)));
    *idPtr = newId;
    luaL_setmetatable (L, "luaDraw.point");
    return 1;
}

int configInterpreter::newLine (lua_State * L) {
    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    std::size_t newId = self->configuredShapeStorage.newLine();
    std::size_t * idPtr = static_cast<size_t*>(lua_newuserdata(L, sizeof(size_t)));
    *idPtr = newId;
    luaL_setmetatable (L, "luaDraw.line");
    return 1;
}

int configInterpreter::newCircle (lua_State * L) {
    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    std::size_t newId = self->configuredShapeStorage.newCircle();
    std::size_t * idPtr = static_cast<size_t*>(lua_newuserdata(L, sizeof(size_t)));
    *idPtr = newId;
    luaL_setmetatable (L, "luaDraw.circle");
    return 1;
}

int configInterpreter::getPointField (lua_State * L) {
    size_t * idPtr = static_cast<size_t*>(luaL_checkudata (L, 1, "luaDraw.point"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        lua_pushnil (L);
        return 1;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::point& p = self->configuredShapeStorage.getPoint (*idPtr);

    if (field == "x" ) {
        lua_pushnumber (L, static_cast<double>(p.x));
        return 1;
    } else if (field == "y") {
        lua_pushnumber (L, static_cast<double>(p.y));
        return 1;
    } else if (field == "name") {
        lua_pushstring (L, p.name.c_str());
        return 1;
    } else if (field == "color") {
        if (p.color == -1) {
            lua_pushnil (L);
            return 1;
        }
        lua_rawgeti (L, LUA_REGISTRYINDEX, self->configuredColorStorage.getColor (p.color).luaIndex);
        return 1;
    }
    lua_pushnil (L);
    return 1;
}

int configInterpreter::setPointField (lua_State * L) {
    size_t * idPtr = static_cast<size_t*>(luaL_checkudata (L, 1, "luaDraw.point"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        return 0;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::point& p = self->configuredShapeStorage.getPoint (*idPtr);

    if (field == "x" ) {
        p.x = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "y") {
        p.y = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "name") {
        p.name = luaL_checkstring (L, 3);
        return 0;
    } else if (field == "color") {
        int * colorIdPtr = static_cast<int*>(luaL_checkudata (L, 3, "luaDraw.color"));
        p.color = *colorIdPtr;
        return 0;
    }
    return 0;
}

int configInterpreter::getLineField (lua_State * L) {
    size_t * idPtr = static_cast<size_t*>(luaL_checkudata (L, 1, "luaDraw.line"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        lua_pushnil (L);
        return 1;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::line& l = self->configuredShapeStorage.getLine (*idPtr);

    if (field == "a" ) {
        lua_pushnumber (L, static_cast<double>(l.a));
        return 1;
    } else if (field == "b") {
        lua_pushnumber (L, static_cast<double>(l.b));
        return 1;
    } else if (field == "c") {
        lua_pushnumber (L, static_cast<double>(l.c));
        return 1;
    } else if (field == "name") {
        lua_pushstring (L, l.name.c_str());
        return 1;
    } else if (field == "color") {
        if (l.color == -1) {
            lua_pushnil (L);
            return 1;
        }
        lua_rawgeti (L, LUA_REGISTRYINDEX, self->configuredColorStorage.getColor (l.color).luaIndex);
        return 1;
    }
    lua_pushnil (L);
    return 1;
}

int configInterpreter::setLineField (lua_State * L) {
    size_t * idPtr = static_cast<size_t*>(luaL_checkudata (L, 1, "luaDraw.line"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        return 0;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::line& l = self->configuredShapeStorage.getLine (*idPtr);

    if (field == "a" ) {
        l.a = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "b") {
        l.b = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "c") {
        l.c = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "name") {
        l.name = luaL_checkstring (L, 3);
        return 0;
    } else if (field == "color") {
        int * colorIdPtr = static_cast<int*>(luaL_checkudata (L, 3, "luaDraw.color"));
        l.color = *colorIdPtr;
        return 0;
    }
    return 0;
}

int configInterpreter::getCircleField (lua_State * L) {
    size_t * idPtr = static_cast<size_t*>(luaL_checkudata (L, 1, "luaDraw.circle"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        lua_pushnil (L);
        return 1;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::circle& c = self->configuredShapeStorage.getCircle (*idPtr);

    if (field == "x" ) {
        lua_pushnumber (L, static_cast<double>(c.x));
        return 1;
    } else if (field == "y") {
        lua_pushnumber (L, static_cast<double>(c.y));
        return 1;
    } else if (field == "r") {
        lua_pushnumber (L, static_cast<double>(c.r));
        return 1;
    } else if (field == "name") {
        lua_pushstring (L, c.name.c_str());
        return 1;
    } else if (field == "color") {
        if (c.color == -1) {
            lua_pushnil (L);
            return 1;
        }
        lua_rawgeti (L, LUA_REGISTRYINDEX, self->configuredColorStorage.getColor (c.color).luaIndex);
        return 1;
    }
    lua_pushnil (L);
    return 1;
}

int configInterpreter::setCircleField (lua_State * L) {
    size_t * idPtr = static_cast<size_t*>(luaL_checkudata (L, 1, "luaDraw.circle"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        return 0;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::circle& c = self->configuredShapeStorage.getCircle (*idPtr);

    if (field == "x" ) {
        c.x = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "y") {
        c.y = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "r") {
        c.r = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "name") {
        c.name = luaL_checkstring (L, 3);
        return 0;
    } else if (field == "color") {
        int * colorIdPtr = static_cast<int*>(luaL_checkudata (L, 3, "luaDraw.color"));
        c.color = *colorIdPtr;
        return 0;
    }
    return 0;
}

int configInterpreter::getTime (lua_State * L) {

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    auto nowTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> passedTime {nowTime - self->startTime};

    lua_pushnumber (L, passedTime.count());
    return 1;
}


int configInterpreter::newColor (lua_State * L) {
    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    luaPushNewColor (L, self->configuredColorStorage);

    return 1;
}

int configInterpreter::setColorField (lua_State * L) {

    int * idPtr = static_cast<int*>(luaL_checkudata (L, 1, "luaDraw.color"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        return 0;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    colorStorage::color& c = self->configuredColorStorage.getColor (*idPtr);

    if (field == "r" ) {
        c.colorComponents[0] = static_cast<float>(luaL_checknumber (L, 3));
    } else if (field == "g") {
        c.colorComponents[1] = static_cast<float>(luaL_checknumber (L, 3));
    } else if (field == "b") {
        c.colorComponents[2] = static_cast<float>(luaL_checknumber (L, 3));
    } else if (field == "a") {
        c.colorComponents[3] = static_cast<float>(luaL_checknumber (L, 3));
    }
    c.updateUniform();
    return 0;
}

int configInterpreter::getColorField (lua_State * L) {
    int * idPtr = static_cast<int*>(luaL_checkudata (L, 1, "luaDraw.color"));

    if (lua_type (L, 2) != LUA_TSTRING) {
        lua_pushnil (L);
        return 1;
    }

    std::string field = lua_tostring(L, 2);

    configInterpreter * self = static_cast<configInterpreter*>(lua_touserdata (L, lua_upvalueindex(1)));
    colorStorage::color& c = self->configuredColorStorage.getColor (*idPtr);

    if (field == "r" ) {
        lua_pushnumber (L, static_cast<double>(c.colorComponents[0]));
        return 1;
    } else if (field == "g") {
        lua_pushnumber (L, static_cast<double>(c.colorComponents[1]));
        return 1;
    } else if (field == "b") {
        lua_pushnumber (L, static_cast<double>(c.colorComponents[2]));
        return 1;
    } else if (field == "a") {
        lua_pushnumber (L, static_cast<double>(c.colorComponents[3]));
        return 1;
    }
    lua_pushnil (L);
    return 1;
}
