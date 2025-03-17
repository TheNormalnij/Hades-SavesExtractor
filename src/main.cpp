//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <iostream>

#include "Hades/HadesSaveConverter.h"
#include "LuaSerialize.h"
#include "GameVersions.h"
#include "LuaReadHelper.h"

constexpr uint32_t SGG_HEADER_MAGIC = 0x31424753;

struct SaveHeader {
    uint32_t magic{};
    uint32_t checkSum{};
    uint32_t gameVersion{};
};

static bool extract(const std::string_view inputFilePath, std::ofstream& outputFile) {
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

    SaveHeader *header = reinterpret_cast<SaveHeader*>(buffer.data());

    if (header->magic != SGG_HEADER_MAGIC) {
        std::cerr << "Invalid header magic value." << std::endl;
        return false;
    }
    
    switch (static_cast<eGameVersion>(header->gameVersion)) {
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

static bool import(const std::string_view inputFile, std::ofstream& outputFile) {
    lua_State *L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state." << std::endl;
        return false;
    }
    
    if (luaL_dofile(L, inputFile.data()) != 0) {
        std::cerr << "Failed to load Lua file." << std::endl;
        lua_close(L);
        return false;
    }

    uint32_t gameVersion = 0;
    LuaReadHelper::readGlobal(L, "GAME_VERSION", gameVersion);

    switch (static_cast<eGameVersion>(gameVersion)) {
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

static void showUsage(const char* cmdName) {
    std::cerr << "Usage: " << cmdName << " --extract <filepath> --out <filepath>" << std::endl;
    std::cerr << "       " << cmdName << " --import <filepath> --out <filepath>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        showUsage(argv[0]);
        return 1;
    }

    std::string_view command = argv[1];
    std::string_view inputFile = argv[2];
    std::string_view outFlag = argv[3];
    std::string_view outputFilePath = argv[4];

    if (command != "--extract" && command != "--import") {
        std::cerr << "Invalid command" << std::endl;
        showUsage(argv[0]);
        return 1;
    }

    if (outFlag != "--out") {
        std::cerr << "Invalid command" << std::endl;
        showUsage(argv[0]);
        return 1;
    }

    std::ofstream outputFile(outputFilePath.data(), std::ifstream::binary);
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    bool result = false;
    if (command == "--extract") {
        result = extract(inputFile, outputFile);
    } else if (command == "--import") {
        result = import(inputFile, outputFile);
    }

    return result ? 0 : 1;
}
