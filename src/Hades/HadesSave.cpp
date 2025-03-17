//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "HadesSave.h"
#include <fstream>
#include <iostream>
#include "../adler32.h"

bool HadesSaveData::read(const std::vector<uint8_t> &buffer) {
    SGGBinaryParser parser{&buffer};
    SAFE_PARSER_READ(magic)
    SAFE_PARSER_READ(checksum)
    SAFE_PARSER_READ(gameVersion)
    SAFE_PARSER_READ(timestamp)
    SAFE_PARSER_READ(location)
    SAFE_PARSER_READ(complectedRuns)
    SAFE_PARSER_READ(accumulatedMetaPoints)
    SAFE_PARSER_READ(activeShrinePoints)
    SAFE_PARSER_READ(easyMode)
    SAFE_PARSER_READ(hardMode)
    SAFE_PARSER_READ(notableLuaData)
    SAFE_PARSER_READ(mapName)
    SAFE_PARSER_READ(mapName2)

    std::string luaBindDataCompressed;
    SAFE_PARSER_READ(luaBindDataCompressed)

    luaBindData.resize(SAVE_BUFFER_SIZE);

    int realSize = LZ4_decompress_safe(luaBindDataCompressed.c_str(), reinterpret_cast<char *>(luaBindData.data()),
                                       luaBindDataCompressed.size(), luaBindData.size());

    if (realSize <= 0)
        return false;

    luaBindData.resize(realSize);

    return true;
}

bool HadesSaveData::write(std::vector<uint8_t> &binary) {
    SGGBinaryWritter writter{&binary};
    SAFE_PARSER_WRITE(magic)
    SAFE_PARSER_WRITE(checksum)
    SAFE_PARSER_WRITE(gameVersion)
    SAFE_PARSER_WRITE(timestamp)
    SAFE_PARSER_WRITE(location)
    SAFE_PARSER_WRITE(complectedRuns)
    SAFE_PARSER_WRITE(accumulatedMetaPoints)
    SAFE_PARSER_WRITE(activeShrinePoints)
    SAFE_PARSER_WRITE(easyMode)
    SAFE_PARSER_WRITE(hardMode)
    SAFE_PARSER_WRITE(notableLuaData)
    SAFE_PARSER_WRITE(mapName)
    SAFE_PARSER_WRITE(mapName2)

    std::string luaBindDataCompressed;
    luaBindDataCompressed.resize(LZ4_COMPRESSBOUND(luaBindData.size()));

    int realSize = LZ4_compress_default(luaBindData.data(), luaBindDataCompressed.data(), luaBindData.size(),
                                        luaBindDataCompressed.size());
    if (realSize <= 0)
        return false;

    luaBindDataCompressed.resize(realSize);

    SAFE_PARSER_WRITE(luaBindDataCompressed)

    uint32_t hash = Adler32::Calculate(binary.data() + 8, binary.size() - 8);
    *reinterpret_cast<uint32_t *>(&binary.at(4)) = hash;

    return true;
}
