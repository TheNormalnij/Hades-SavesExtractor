//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "HadesSave.h"
#include <fstream>
#include <iostream>
#include "adler32.h"

bool HadesSaveData::read(const std::string &filePath) {

    std::ifstream inFile(filePath, std::ifstream::ate | std::ifstream::binary);
    if (!inFile) {
        std::cerr << "Error opening input file." << std::endl;
        return false;
    }

    std::vector<uint8_t> buffer{};
    buffer.resize(inFile.tellg());
    inFile.seekg(0);
    inFile.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    inFile.close();

    SGGBinaryParser parser{&buffer};

    if (!parser.read(magic))
        return false;

    if (!parser.read(checksum))
        return false;

    if (!parser.read(gameVersion))
        return false;

    if (!parser.read(timestamp))
        return false;

    if (!parser.read(location))
        return false;

    if (!parser.read(complectedRuns))
        return false;

    if (!parser.read(accumulatedMetaPoints))
        return false;

    if (!parser.read(activeShrinePoints))
        return false;

    if (!parser.read(easyMode))
        return false;

    if (!parser.read(hardMode))
        return false;

    if (!parser.read(notableLuaData))
        return false;

    if (!parser.read(mapName))
        return false;

    if (!parser.read(mapName2))
        return false;

    std::string luaBindDataCompressed;
    if (!parser.read(luaBindDataCompressed))
        return false;

    luaBindData.resize(SAVE_BUFFER_SIZE);

    int realSize = LZ4_decompress_safe(luaBindDataCompressed.c_str(), reinterpret_cast<char *>(luaBindData.data()),
                                       luaBindDataCompressed.size(), luaBindData.size());

    if (realSize <= 0)
        return false;

    luaBindData.resize(realSize);

    return true;
}

bool HadesSaveData::write(const std::string &filePath) {
    std::ofstream outFile(filePath, std::ifstream::binary);
    if (!outFile) {
        std::cerr << "Error opening output file." << std::endl;
        return false;
    }

    std::vector<uint8_t> binary{};

    SGGBinaryWritter writer{&binary};
    if (!writer.write(magic))
        return false;
    if (!writer.write(checksum))
        return false;
    if (!writer.write(gameVersion))
        return false;
    if (!writer.write(timestamp))
        return false;
    if (!writer.write(location))
        return false;
    if (!writer.write(complectedRuns))
        return false;
    if (!writer.write(accumulatedMetaPoints))
        return false;
    if (!writer.write(activeShrinePoints))
        return false;
    if (!writer.write(easyMode))
        return false;
    if (!writer.write(hardMode))
        return false;
    if (!writer.write(notableLuaData))
        return false;
    if (!writer.write(mapName))
        return false;
    if (!writer.write(mapName2))
        return false;

    std::string luaBindDataCompressed;
    luaBindDataCompressed.resize(LZ4_COMPRESSBOUND(luaBindData.size()));

    int realSize = LZ4_compress_default(luaBindData.data(), luaBindDataCompressed.data(), luaBindData.size(),
                                        luaBindDataCompressed.size());
    if (realSize <= 0)
        return false;

    luaBindDataCompressed.resize(realSize);

    if (!writer.write(luaBindDataCompressed))
        return false;

    uint32_t hash = Adler32::Calculate(binary.data() + 8, binary.size() - 8);
    *reinterpret_cast<uint32_t *>(&binary.at(4)) = hash;

    outFile.write(reinterpret_cast<const char *>(binary.data()), binary.size());

    return true;
}
