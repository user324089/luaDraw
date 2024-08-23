#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <map>

class callback {
    private:
        int luaId = -1;
        lua_State * L = nullptr;
    public:
        callback () = default;
        callback (const callback & r) = delete;
        callback & operator = (const callback & r) = delete;
        callback (callback && r) :luaId (r.luaId) {
            r.luaId = -1;
        }
        void unref () {
            if (luaId != -1) {
                luaL_unref (L, LUA_REGISTRYINDEX, luaId);
                luaId = -1;
            }
        }
        callback (lua_State * _L) : L (_L) {}
        void getFromStack () {
            unref ();
            luaId = luaL_ref (L, LUA_REGISTRYINDEX);
        }
        void call () {
            if (luaId == -1) return;
            lua_rawgeti (L, LUA_REGISTRYINDEX, luaId);
            lua_pcall (L, 0, 0, 0);
        }
        ~callback () {}
};

class callbackStorage {
    private:
        std::map<char, callback> mapKeyToCallback;
    public:
        void addCallbackFromStack (lua_State * L, char key) {
            mapKeyToCallback.emplace (key, L);
            mapKeyToCallback[key].getFromStack ();
        }
        void call (char key) {
            if (std::map<char, callback>::iterator iter;
                    (iter = mapKeyToCallback.find (key)) != mapKeyToCallback.end()) {
                iter->second.call ();
            }
        }
        void unrefAllCallbacks () {
            for (auto & [key, val] : mapKeyToCallback) {
                val.unref ();
            }
            mapKeyToCallback.clear();
        }
};
