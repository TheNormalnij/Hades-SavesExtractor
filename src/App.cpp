//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "App.h"

#include "GameVersions.h"
#include "Hades/HadesSaveConverter.h"
#include "LuaReadHelper.h"
#include "LuaSerialize.h"
#include <iostream>

constexpr uint32_t SGG_HEADER_MAGIC = 0x31424753;

struct SaveHeader {
    uint32_t magic{};
    uint32_t checkSum{};
    uint32_t gameVersion{};
};

bool App::extract(const std::string_view inputFilePath, std::ofstream &outputFile) {
    std::ifstream inFile(inputFilePath.data(), std::ifstream::ate | std::ifstream::binary);
    if (!inFile) {
        std::cerr << "Error opening input file." << std::endl;
        return false;
    }

    std::vector<uint8_t> buffer{};
    buffer.resize(inFile.tellg());
    inFile.seekg(0);
    inFile.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    inFile.close();

    if (buffer.size() < sizeof(SaveHeader)) {
        std::cerr << "Invalid header size." << std::endl;
        return false;
    }

    SaveHeader *header = reinterpret_cast<SaveHeader *>(buffer.data());

    if (header->magic != SGG_HEADER_MAGIC) {
        std::cerr << "Invalid header magic value." << std::endl;
        return false;
    }

    switch (static_cast<eGameVersion>(header->gameVersion)) {
    case eGameVersion::HADES2:
    case eGameVersion::HADES: {
        HadesSaveConverter::ToLua(buffer, outputFile);
        break;
    }
    default:
        std::cerr << "Unknown game version." << std::endl;
        return false;
    }

    return true;
}

bool App::import(const std::string_view inputFile, std::ofstream &outputFile) {
    lua_State *L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return false;
    }

    if (luaL_dofile(L, inputFile.data()) != LUA_OK) {
        std::cerr << "Failed to load Lua file." << std::endl;
        lua_close(L);
        return false;
    }

    uint32_t gameVersion = 0;
    LuaReadHelper::readGlobal(L, "GAME_VERSION", gameVersion);

    switch (static_cast<eGameVersion>(gameVersion)) {
    case eGameVersion::HADES2:
    case eGameVersion::HADES:
        HadesSaveConverter::FromLua(L, outputFile);
        break;
    default:
        std::cerr << "Unknown game version." << std::endl;
        return false;
    }

    lua_close(L);

    return true;
}