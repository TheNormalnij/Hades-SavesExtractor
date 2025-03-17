//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "LuaSerialize.h"

namespace LuaFileWriteHelper {

static void writeGlobal(std::ofstream &stream, const char *globalName, bool value) {
    stream << globalName << " = " << (value ? "true" : "false") << std::endl;
}

static void writeGlobal(std::ofstream &stream, const char *globalName, const std::vector<std::string> &value) {
    stream << globalName << " = {" << std::endl;
    for (const auto &str : value) {
        stream << "    \"" << str << "\"," << std::endl;
    }
    stream << "}" << std::endl;
}

static bool writeGlobalLuaBind(std::ofstream &stream, const char *globalName, const std::string &value) {
    lua_State *L = luaL_newstate();

    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return false;
    }

    int count;
    if (luabins_load(L, reinterpret_cast<const uint8_t *>(value.c_str()), value.size(), &count) != LUABINS_ESUCCESS) {
        std::cerr << "Failed to deserialise." << std::endl;
        lua_close(L);
        return false;
    }

    if (!lua_istable(L, 1)) {
        std::cerr << "Sealized value is not a table." << std::endl;
        lua_close(L);
        return false;
    }

    stream << globalName << " = {" << std::endl;

    LuaSerialize::serialize_table(L, stream, 1, 1);

    stream << "}" << std::endl;

    lua_close(L);

    return true;
}

template <typename T, typename = std::enable_if<std::is_convertible<T, int>::value>>
static void writeGlobal(std::ofstream &stream, const char *globalName, T value) {
    stream << globalName << " = " << value << std::endl;
}
} // namespace LuaFileWriteHelper
