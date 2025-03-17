//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <saveload.h>

namespace LuaReadHelper {

static bool readGlobalLuaBind(lua_State *L, const char *name, std::string &out) {

    lua_getglobal(L, name);

    if (!lua_istable(L, -1)) {
        std::cerr << name << " is not a table." << std::endl;
        lua_close(L);
        return false;
    }

    if (luabins_save(L, 1, 1) != LUABINS_ESUCCESS) {
        std::cerr << "Failed to save Lua state." << std::endl;
        lua_pop(L, 1);
        return false;
    }

    size_t len = 0;
    const char *bindData = lua_tolstring(L, -1, &len);
    out = {bindData, len};
    lua_pop(L, 1);

    lua_pop(L, 1);

    return true;
}

static bool readGlobal(lua_State *L, const char *name, bool &out) {
    lua_getglobal(L, name);
    out = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return true;
}

static bool readGlobal(lua_State *L, const char *name, std::string &out) {
    lua_getglobal(L, name);
    out = lua_tostring(L, -1);
    lua_pop(L, 1);

    return true;
}

static bool readGlobal(lua_State *L, const char *name, std::vector<std::string> &out) {
    lua_getglobal(L, name);
    if (!lua_istable(L, -1)) {
        std::cerr << name << " is not a table." << std::endl;
        lua_close(L);
        return false;
    }

    int count = luaL_len(L, -1);
    for (int i = 0; i < count; i++) {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, -2);
        if (lua_isstring(L, -1)) {
            out.push_back(lua_tostring(L, -1));
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    return true;
}

template <typename T> static bool readGlobal(lua_State *L, const char *name, T &out) {
    lua_getglobal(L, name);
    out = lua_tonumber(L, -1);
    lua_pop(L, 1);

    return true;
}
} // namespace LuaReadHelper