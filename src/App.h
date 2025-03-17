//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <fstream>
#include <string>

namespace App {
bool extract(const std::string_view inputFilePath, std::ofstream &outputFile);
bool import(const std::string_view inputFile, std::ofstream &outputFile);
}