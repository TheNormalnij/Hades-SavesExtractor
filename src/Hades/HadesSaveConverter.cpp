//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "HadesSaveConverter.h"

#include <iostream>
#include <saveload.h>

#include "../LuaFileWriteHelper.h"
#include "../LuaReadHelper.h"
#include "../LuaSerialize.h"

bool HadesSaveConverter::ToLua(std::vector<uint8_t> &buffer, std::ofstream &outFile) {
    HadesSaveData save{};

    if (!save.read(buffer)) {
        std::cerr << "Failed to load save." << std::endl;
        return false;
    }

    LuaFileWriteHelper::writeGlobal(outFile, "MAGIC", save.magic);
    LuaFileWriteHelper::writeGlobal(outFile, "CHECKSUM", save.checksum);
    LuaFileWriteHelper::writeGlobal(outFile, "GAME_VERSION", save.gameVersion);
    LuaFileWriteHelper::writeGlobal(outFile, "TIMESTAMP", save.timestamp);
    LuaFileWriteHelper::writeGlobal(outFile, "LOCATION", save.location);
    LuaFileWriteHelper::writeGlobal(outFile, "COMPLETED_RUNS", save.complectedRuns);
    LuaFileWriteHelper::writeGlobal(outFile, "ACCUMULATED_META_POINTS", save.accumulatedMetaPoints);
    LuaFileWriteHelper::writeGlobal(outFile, "ACTIVE_SHRINE_POINTS", save.activeShrinePoints);
    LuaFileWriteHelper::writeGlobal(outFile, "EASY_MODE", static_cast<bool>(save.easyMode));
    LuaFileWriteHelper::writeGlobal(outFile, "HARD_MODE", static_cast<bool>(save.hardMode));
    LuaFileWriteHelper::writeGlobal(outFile, "MAP_NAME", save.mapName);
    LuaFileWriteHelper::writeGlobal(outFile, "MAP_NAME2", save.mapName2);
    LuaFileWriteHelper::writeGlobal(outFile, "NOTABLE_LUA_DATA", save.notableLuaData);
    LuaFileWriteHelper::writeGlobalLuaBind(outFile, "LUA_DATA", save.luaBindData);

    return true;
}

bool HadesSaveConverter::FromLua(lua_State *L, std::ofstream &outputFile) {
    HadesSaveData save;

    LuaReadHelper::readGlobal(L, "MAGIC", save.magic);
    LuaReadHelper::readGlobal(L, "CHECKSUM", save.checksum);
    LuaReadHelper::readGlobal(L, "GAME_VERSION", save.gameVersion);
    LuaReadHelper::readGlobal(L, "TIMESTAMP", save.timestamp);
    LuaReadHelper::readGlobal(L, "LOCATION", save.location);
    LuaReadHelper::readGlobal(L, "COMPLETED_RUNS", save.complectedRuns);
    LuaReadHelper::readGlobal(L, "ACCUMULATED_META_POINTS", save.accumulatedMetaPoints);
    LuaReadHelper::readGlobal(L, "ACTIVE_SHRINE_POINTS", save.activeShrinePoints);
    LuaReadHelper::readGlobal(L, "EASY_MODE", save.easyMode);
    LuaReadHelper::readGlobal(L, "HARD_MODE", save.hardMode);
    LuaReadHelper::readGlobal(L, "MAP_NAME", save.mapName);
    LuaReadHelper::readGlobal(L, "MAP_NAME2", save.mapName2);
    LuaReadHelper::readGlobal(L, "NOTABLE_LUA_DATA", save.notableLuaData);
    LuaReadHelper::readGlobalLuaBind(L, "LUA_DATA", save.luaBindData);

    std::vector<uint8_t> binaryData{};
    if (!save.write(binaryData)) {
        std::cerr << "Failed to write save." << std::endl;
    }

    outputFile.write(reinterpret_cast<const char *>(binaryData.data()), binaryData.size());

    return true;
}
