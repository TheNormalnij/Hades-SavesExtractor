//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <string>
#include <vector>
#include <lz4.h>
#include "SGGBinaryParser.h"
#include "SGGBinaryWritter.h"

constexpr size_t SAVE_BUFFER_SIZE = 3129344 * 2;

class HadesSaveData {
  public:
    bool read(const std::string &filePath);
    bool write(const std::string &filePath);

    uint32_t magic;       // SGB1
    uint32_t checksum;    // adler32 ?
    uint32_t gameVersion; // 0x10 in Hades
    uint64_t timestamp;

    std::string location;
    uint32_t complectedRuns;
    uint32_t accumulatedMetaPoints;
    uint32_t activeShrinePoints;
    uint8_t easyMode;
    uint8_t hardMode;
    std::vector<std::string> notableLuaData;
    std::string mapName;
    std::string mapName2;
    std::string luaBindData;
};
