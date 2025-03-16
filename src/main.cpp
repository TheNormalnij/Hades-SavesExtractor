//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <iostream>

#include "HadesSave.h"
#include "LuaSerialize.h"

static void extract(const std::string &inputFile, const std::string &outputFile) {
    HadesSaveData save{};

    if (!save.read(inputFile)) {
        std::cerr << "Failed to load save." << std::endl;
        return;
    }

    lua_State *L = luaL_newstate();

    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return;
    }

    int count;
    if (luabins_load(L, reinterpret_cast<const uint8_t *>(save.luaBindData.c_str()), save.luaBindData.size(), &count) !=
        0) {
        std::cerr << "Failed to deserialise." << std::endl;
        lua_close(L);
        return;
    }

    int top = lua_gettop(L);

    if (!lua_istable(L, 1)) {
        std::cerr << "Sealized value is not a table." << std::endl;
        lua_close(L);
        return;
    }

    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error opening file(s)." << std::endl;
        return;
    }

    outFile << "MAGIC = " << save.magic << std::endl;
    outFile << "CHECKSUM = " << save.checksum << std::endl;
    outFile << "GAME_VERSION = " << save.gameVersion << std::endl;
    outFile << "TIMESTAMP = " << save.timestamp << std::endl;
    outFile << "LOCATION = \"" << save.location << "\"" << std::endl;
    outFile << "COMPLETED_RUNS = " << save.complectedRuns << std::endl;
    outFile << "ACCUMULATED_META_POINTS = " << save.accumulatedMetaPoints << std::endl;
    outFile << "ACTIVE_SHRINE_POINTS = " << save.activeShrinePoints << std::endl;
    outFile << "EASY_MODE = " << (save.easyMode ? "true" : "false") << std::endl;
    outFile << "HARD_MODE = " << (save.hardMode ? "true" : "false") << std::endl;
    outFile << "MAP_NAME = \"" << save.mapName << "\"" << std::endl;
    outFile << "MAP_NAME2 = \"" << save.mapName2 << "\"" << std::endl;

    outFile << "NOTABLE_LUA_DATA = {" << std::endl;
    for (const auto &str : save.notableLuaData) {
        outFile << "    \"" << str << "\"," << std::endl;
    }
    outFile << "}" << std::endl;

    outFile << "LUA_DATA = {" << std::endl;

    LuaSerialize::serialize_table(L, outFile, 1, 1);

    outFile << "}" << std::endl;

    lua_close(L);
}

static void import(const std::string& inputFile, const std::string& outputFile) {
    lua_State *L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return;
    }
    
    if (luaL_dofile(L, inputFile.c_str()) != 0) {
        std::cerr << "Failed to load save." << std::endl;
        lua_close(L);
        return;
    }
    
    HadesSaveData data{};
    lua_getglobal(L, "MAGIC");
    data.magic = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "GAME_VERSION");
    data.gameVersion = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "TIMESTAMP");
    data.timestamp = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "LOCATION");
    data.location = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "COMPLETED_RUNS");
    data.complectedRuns = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "ACCUMULATED_META_POINTS");
    data.accumulatedMetaPoints = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "ACTIVE_SHRINE_POINTS");
    data.activeShrinePoints = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "EASY_MODE");
    data.easyMode = lua_toboolean(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "HARD_MODE");
    data.hardMode = lua_toboolean(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "MAP_NAME");
    data.mapName = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "MAP_NAME2");
    data.mapName2 = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "NOTABLE_LUA_DATA");
    if (!lua_istable(L, -1)) {
        std::cerr << "NOTABLE_LUA_DATA is not a table." << std::endl;
        lua_close(L);
        return;
    }

    int count = luaL_len(L, -1);
    for (int i = 0; i < count; i++) {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, -2);
        if (lua_isstring(L, -1)) {
            data.notableLuaData.push_back(lua_tostring(L, -1));
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    lua_getglobal(L, "LUA_DATA");

    if (!lua_istable(L, -1)) {
        std::cerr << "LUA_DATA is not a table." << std::endl;
        lua_close(L);
        return;
    }

    if (luabins_save(L, 1, 1) == 0) {
        size_t len = 0;
        const char *bindData = lua_tolstring(L, -1, &len);
        data.luaBindData = {bindData, len};
        lua_pop(L, 1);
    } else {
        std::cerr << "Failed to save Lua state." << std::endl;
    }
    lua_pop(L, 1);

    if (!data.write(outputFile)) {
        std::cerr << "Failed to write save." << std::endl;
    }

    lua_close(L);
}

int main(int argc, char* argv[]) {    
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " --extract <filepath> --out <filepath>" << std::endl;
        std::cerr << "       " << argv[0] << " --import <filepath> --out <filepath>" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    std::string inputFile = argv[2];
    std::string outFlag = argv[3];
    std::string outputFile = argv[4];

    if (outFlag != "--out") {
        std::cerr << "Invalid output flag. Use --out <filepath>" << std::endl;
        return 1;
    }

    if (command == "--extract") {
        extract(inputFile, outputFile);
    } else if (command == "--import") {
        import(inputFile, outputFile);
    } else {
        std::cerr << "Invalid command. Use --extract or --import" << std::endl;
        return 1;
    }

    return 0;
}
