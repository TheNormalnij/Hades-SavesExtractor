//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "App.h"
#include <fstream>
#include <iostream>
#include <filesystem>

#include <lua.hpp>

int main() {
	std::ofstream out{"./Hades2ParseTestOut.lua"};

	if (!out) {
        std::cerr << "Cannot create output file";
		return 1;
	}

	bool result = App::extract("./Hades2.sav", out);

	if (!result) {
		return 1;
	}

	lua_State *L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return false;
    }

    if (luaL_dofile(L, "./Hades2ParseTestOut.lua") != LUA_OK) {
        std::cerr << "Result is not a valid Lua file." << std::endl;
        lua_close(L);
        return 1;
    }
    
    lua_close(L);

    std::filesystem::remove("./Hades2ParseTestOut.lua");

    return result ? 0 : 1;
}
