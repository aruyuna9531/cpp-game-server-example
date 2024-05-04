#pragma once

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

class LuaVm {
private:
    lua_State *luaState;
    int id = 0;
public:
    LuaVm();
    ~LuaVm();
}