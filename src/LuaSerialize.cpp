//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "LuaSerialize.h"

static std::string toSafeString(lua_State *L, int key_pos) {
    size_t len = 0;
    const char *str = lua_tolstring(L, key_pos, &len);
    std::string safeString{};
    safeString.reserve(len);
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\\') {
            safeString += "\\\\";
        } else {
            safeString += str[i];
        }
    }
    return safeString;
}

bool LuaSerialize::serialize_table(lua_State *L, std::ofstream &outFile, int index, int nesting) {
    int result = true;

    lua_checkstack(L, 2);
    lua_pushnil(L);

    std::string nestOffset = "";
    for (int i = 0; i < nesting; i++) {
        nestOffset += "    ";
    }

    while (lua_next(L, index) != 0) {
        int value_pos = lua_gettop(L); /* We need absolute values */
        int key_pos = value_pos - 1;

        outFile << nestOffset;

        if (lua_isnumber(L, key_pos)) {
            lua_pushvalue(L, key_pos);
            outFile << "[" << lua_tostring(L, -1) << "] = ";
            lua_pop(L, 1);
        } else {
            outFile << "[\"" << toSafeString(L, key_pos) << "\"] = ";
        }

        if (result) {
            result = serialize_value(L, outFile, value_pos, nesting);
        }

        if (result) {
            lua_pop(L, 1);
        }
    }

    return result;
}

int LuaSerialize::serialize_value(lua_State *L, std::ofstream &outFile, int index, int nesting) {
    bool result = true;

    switch (lua_type(L, index)) {
    case LUA_TNIL:
        outFile << "nil;";
        break;

    case LUA_TBOOLEAN:
        outFile << (lua_toboolean(L, index) ? "true;" : "false;");
        break;

    case LUA_TNUMBER:
        outFile << lua_tostring(L, index) << ";";
        break;

    case LUA_TSTRING:
        outFile << "\"" << toSafeString(L, index) << "\";";
        break;

    case LUA_TTABLE:
        outFile << "{" << std::endl;
        result = serialize_table(L, outFile, index, nesting + 1);

        for (int i = 0; i < nesting; i++) {
            outFile << "    ";
        }

        outFile << "};";
        break;

    default:
        result = false;
    }

    outFile << std::endl;

    return result;
}