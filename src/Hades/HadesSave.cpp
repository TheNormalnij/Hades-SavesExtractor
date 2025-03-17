//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "HadesSave.h"
#include "../adler32.h"
#include <fstream>
#include <iostream>

bool HadesSaveData::read(const std::vector<uint8_t> &buffer) {
    SGGBinaryParser parser{&buffer};
    SAFE_PARSER_READ(parser, magic)
    SAFE_PARSER_READ(parser, checksum)
    SAFE_PARSER_READ(parser, gameVersion)
    SAFE_PARSER_READ(parser, timestamp)
    SAFE_PARSER_READ(parser, location)
    SAFE_PARSER_READ(parser, complectedRuns)
    SAFE_PARSER_READ(parser, accumulatedMetaPoints)
    SAFE_PARSER_READ(parser, activeShrinePoints)
    SAFE_PARSER_READ(parser, easyMode)
    SAFE_PARSER_READ(parser, hardMode)
    SAFE_PARSER_READ(parser, notableLuaData)
    SAFE_PARSER_READ(parser, mapName)
    SAFE_PARSER_READ(parser, mapName2)

    std::string luaBindDataCompressed;
    SAFE_PARSER_READ(parser, luaBindDataCompressed)

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
    SAFE_PARSER_WRITE(writter, magic)
    SAFE_PARSER_WRITE(writter, checksum)
    SAFE_PARSER_WRITE(writter, gameVersion)
    SAFE_PARSER_WRITE(writter, timestamp)
    SAFE_PARSER_WRITE(writter, location)
    SAFE_PARSER_WRITE(writter, complectedRuns)
    SAFE_PARSER_WRITE(writter, accumulatedMetaPoints)
    SAFE_PARSER_WRITE(writter, activeShrinePoints)
    SAFE_PARSER_WRITE(writter, easyMode)
    SAFE_PARSER_WRITE(writter, hardMode)
    SAFE_PARSER_WRITE(writter, notableLuaData)
    SAFE_PARSER_WRITE(writter, mapName)
    SAFE_PARSER_WRITE(writter, mapName2)

    std::string luaBindDataCompressed;
    luaBindDataCompressed.resize(LZ4_COMPRESSBOUND(luaBindData.size()));

    int realSize = LZ4_compress_default(luaBindData.data(), luaBindDataCompressed.data(), luaBindData.size(),
                                        luaBindDataCompressed.size());
    if (realSize <= 0)
        return false;

    luaBindDataCompressed.resize(realSize);

    SAFE_PARSER_WRITE(writter, luaBindDataCompressed)

    uint32_t hash = Adler32::Calculate(binary.data() + 8, binary.size() - 8);
    *reinterpret_cast<uint32_t *>(&binary.at(4)) = hash;

    return true;
}
