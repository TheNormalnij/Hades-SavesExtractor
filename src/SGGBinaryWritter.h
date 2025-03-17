//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <string>
#include <vector>

#define SAFE_PARSER_WRITE(value)                                                                                       \
    if (!writter.write(value)) {                                                                                       \
        std::cerr << "Cannot write " #value << std::endl;                                                              \
        return false;                                                                                                  \
    }

class SGGBinaryWritter {
  public:
    SGGBinaryWritter(std::vector<uint8_t> *binary) : m_binary{binary} {};

    bool write(void *dest, size_t count) {
        uint8_t *destPtr = static_cast<uint8_t *>(dest);
        for (size_t i = 0; i < count; i++) {
            m_binary->push_back(*destPtr);
            destPtr++;
        }
        return true;
    }

    bool write(std::string &str) {
        uint32_t strlLen = str.size();
        if (!write(strlLen))
            return false;

        return write(static_cast<void *>(str.data()), strlLen);
    }

    template <typename T, typename = std::enable_if<std::is_convertible<T, uint32_t>::value>>
    bool write(T &v) {
        return write(static_cast<void *>(&v), sizeof(T));
    }

    template <typename T> bool write(std::vector<T> &vec) {
        uint32_t vecLen = vec.size();
        if (!write(vecLen))
            return false;

        for (auto &v : vec) {
            if (!write(v))
                return false;
        }

        return true;
    }

  private:
    std::vector<uint8_t>* m_binary;
};