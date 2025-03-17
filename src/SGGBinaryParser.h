//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#define SAFE_PARSER_READ(reader, value)                                                                                \
    if (!reader.read(value)) {                                                                                         \
        std::cerr << "Cannot read " #value << std::endl;                                                               \
        return false;                                                                                                  \
    }

class SGGBinaryParser {
  public:
    SGGBinaryParser(const std::vector<uint8_t> *binary) : m_binary(binary), m_pos{0}, m_maxPos{binary->size()} {};

    bool read(void *dest, size_t count) {
        if (m_maxPos < m_pos + count)
            return false;

        std::memcpy(dest, m_binary->data() + m_pos, count);

        m_pos += count;

        return true;
    }

    bool read(std::string &str) {
        uint32_t strlLen;
        if (!read(strlLen))
            return false;

        str.resize(strlLen);
        return read(static_cast<void *>(str.data()), strlLen);
    }

    template <typename T, typename = std::enable_if<std::is_convertible<T, uint32_t>::value>> bool read(T &v) {
        if (m_pos + sizeof(T) > m_maxPos)
            return false;

        return read(static_cast<void *>(&v), sizeof(T));
    }

    template <typename T> bool read(std::vector<T> &vec) {
        uint32_t vecLen;
        if (!read(vecLen))
            return false;
        vec.resize(vecLen);
        for (auto &v : vec) {
            if (!read(v))
                return false;
        }

        return true;
    }

  private:
    const std::vector<uint8_t> *m_binary;
    size_t m_pos;
    size_t m_maxPos;
};
