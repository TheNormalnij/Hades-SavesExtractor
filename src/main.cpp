//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <iostream>
#include "App.h"

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
        result = App::extract(inputFile, outputFile);
    } else if (command == "--import") {
        result = App::import(inputFile, outputFile);
    }

    return result ? 0 : 1;
}
