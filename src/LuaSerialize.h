//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <fstream>

#include <lua.hpp>

namespace LuaSerialize {

int serialize_value(lua_State *L, std::ofstream &outFile, int index, int nesting);
bool serialize_table(lua_State *L, std::ofstream &outFile, int index, int nesting);

} // namespace LuaSerialize