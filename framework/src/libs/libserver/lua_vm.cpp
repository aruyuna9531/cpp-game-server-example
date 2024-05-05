#include "lua_vm.h"
#include "log4_help.h"
#include <string>

LuaVm::LuaVm() {
    LOG_INFO("Lua Vm init");
    luaState = luaL_newstate();
    if (luaState == nullptr) {
        LOG_FATAL("lua vm init failed!");
        return;
    }
    luaL_openlibs(luaState);
    std::string filename = "../../../../service/main/init.lua";
    if (luaL_dofile(luaState, filename.data()) == 1) {
        LOG_FATAL("Lua Vm init failed!");
        lua_close(luaState);
        luaState = nullptr;
        return;
    }
    // 禁掉lua的OS操作
    static const char *sandbox_env = "_G.os.execute = nil\n"
                                 "_G.os.exit = nil\n"
                                 "_G.os.remove = nil\n"
                                 "_G.os.rename = nil\n"
                                 "_G.io = nil\n"
                                 "_G.debug = nil\n";

    luaL_dostring(luaState, sandbox_env);

    lua_getglobal(luaState, "OnInit");  // 调用lua文件的全局函数OnInit
    lua_pushinteger(luaState, id);
    // pcall参数：1 L 虚拟机对象，2 nArgs Lua函数参数个数，3 nResults lua函数返回值个数，msgH 调用失败的处理方式
    if (lua_pcall(luaState, 1, 0, 0) != 0) {
        LOG_FATAL("Lua Vm init failed! cannot call OnInit in lua script, errcode: " << lua_tostring(luaState, -1));
        lua_close(luaState);
        luaState = nullptr;
        return;
    }
}

LuaVm::~LuaVm() {
    LOG_INFO("Lua Vm destroy");
    if (luaState != nullptr) {
        lua_getglobal(luaState, "OnExit");
        if (lua_pcall(luaState, 0, 0, 0) != 0) {
            LOG_FATAL("Lua Vm init failed! cannot call OnExit in lua script, errcode: " << lua_tostring(luaState, -1));
        }
        lua_close(luaState);
        luaState = nullptr;
    }
}