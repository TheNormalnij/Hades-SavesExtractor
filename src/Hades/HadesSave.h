//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "../SGGBinaryParser.h"
#include "../SGGBinaryWritter.h"
#include <lz4.h>
#include <string>
#include <vector>
#include "../GameVersions.h"

class HadesSaveData {
  public:
    bool read(const std::vector<uint8_t> &buffer);
    bool write(std::vector<uint8_t> &binary);

    eGameVersion getGameVersion() const noexcept { return static_cast<eGameVersion>(gameVersion); };

    uint32_t magic;
    uint32_t checksum;
    uint32_t gameVersion;
    uint64_t timestamp;

    std::string location;
    uint32_t complectedRuns;
    uint32_t accumulatedMetaPoints;
    uint32_t activeShrinePoints;
    uint32_t metaUpgradeLevel; // Hades 2
    uint8_t easyMode;
    uint8_t hardMode;
    std::vector<std::string> notableLuaData;
    std::string mapName;
    std::string mapName2;
    std::string luaBindData;
};
