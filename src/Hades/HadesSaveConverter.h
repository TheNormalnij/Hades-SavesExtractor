//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "HadesSave.h"
#include <lua.hpp>

namespace HadesSaveConverter {
bool ToLua(std::vector<uint8_t> &buffer, std::ofstream &outputFile);
bool FromLua(lua_State *luaState, std::ofstream &outputFile);
} // namespace HadesSaveConverter