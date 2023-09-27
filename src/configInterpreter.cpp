#include "configInterpreter.hpp"
#include <stdexcept>
#include <string>

configInterpreter::configInterpreter (shapeStorage & _configuredShapeStorage) : configuredShapeStorage (_configuredShapeStorage) {
    L = luaL_newstate();
    luaL_openlibs (L);

    luaL_newlibtable (L, luaDrawModuleFunctions);

    lua_pushlightuserdata (L, &configuredShapeStorage);
    lua_pushlightuserdata (L, &startTime);
    luaL_setfuncs (L, luaDrawModuleFunctions, 2);

    lua_setglobal (L, "luaDraw");

    luaL_newmetatable (L, "luaDraw.point");
    lua_pushlightuserdata (L, &configuredShapeStorage);
    luaL_setfuncs (L, pointMetaTableFunctions, 1);
    lua_pop (L, 1);

    luaL_newmetatable (L, "luaDraw.line");
    lua_pushlightuserdata (L, &configuredShapeStorage);
    luaL_setfuncs (L, lineMetaTableFunctions, 1);
    lua_pop (L, 1);

    luaL_newmetatable (L, "luaDraw.circle");
    lua_pushlightuserdata (L, &configuredShapeStorage);
    luaL_setfuncs (L, circleMetaTableFunctions, 1);
    lua_pop (L, 1);
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
    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    std::size_t newId = storagePtr->newPoint();
    std::size_t * idPtr = static_cast<size_t*>(lua_newuserdata(L, sizeof(size_t)));
    *idPtr = newId;
    luaL_setmetatable (L, "luaDraw.point");
    return 1;
}

int configInterpreter::newLine (lua_State * L) {
    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    std::size_t newId = storagePtr->newLine();
    std::size_t * idPtr = static_cast<size_t*>(lua_newuserdata(L, sizeof(size_t)));
    *idPtr = newId;
    luaL_setmetatable (L, "luaDraw.line");
    return 1;
}

int configInterpreter::newCircle (lua_State * L) {
    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    std::size_t newId = storagePtr->newCircle();
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

    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::point& p = storagePtr->getPoint (*idPtr);

    if (field == "x" ) {
        lua_pushnumber (L, static_cast<double>(p.x));
        return 1;
    } else if (field == "y") {
        lua_pushnumber (L, static_cast<double>(p.y));
        return 1;
    } else if (field == "name") {
        lua_pushstring (L, p.name.c_str());
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

    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::point& p = storagePtr->getPoint (*idPtr);

    if (field == "x" ) {
        p.x = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "y") {
        p.y = static_cast<float>(luaL_checknumber (L, 3));
        return 0;
    } else if (field == "name") {
        p.name = luaL_checkstring (L, 3);
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

    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::line& l = storagePtr->getLine (*idPtr);

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

    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::line& l = storagePtr->getLine (*idPtr);

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

    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::circle& c = storagePtr->getCircle (*idPtr);

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

    shapeStorage * storagePtr = static_cast<shapeStorage*>(lua_touserdata (L, lua_upvalueindex(1)));
    shapeStorage::circle& c = storagePtr->getCircle (*idPtr);

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
    }
    return 0;
}

int configInterpreter::getTime (lua_State * L) {

    auto startTimePtr = static_cast<decltype(startTime)*>(lua_touserdata (L, lua_upvalueindex(2)));
    auto nowTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> passedTime {nowTime - *startTimePtr};

    lua_pushnumber (L, passedTime.count());
    return 1;
}
