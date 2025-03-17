// adler32.cpp - originally written and placed in the public domain by Wei Dai

#include "adler32.h"

uint32_t Adler32::Calculate(const uint8_t *input, size_t length) {
    const unsigned long BASE = 65521;

    unsigned long s1 = 1;
    unsigned long s2 = 0;

    if (length % 8 != 0) {
        do {
            s1 += *input++;
            s2 += s1;
            length--;
        } while (length % 8 != 0);

        if (s1 >= BASE)
            s1 -= BASE;
        s2 %= BASE;
    }

    while (length > 0) {
        s1 += input[0];
        s2 += s1;
        s1 += input[1];
        s2 += s1;
        s1 += input[2];
        s2 += s1;
        s1 += input[3];
        s2 += s1;
        s1 += input[4];
        s2 += s1;
        s1 += input[5];
        s2 += s1;
        s1 += input[6];
        s2 += s1;
        s1 += input[7];
        s2 += s1;

        length -= 8;
        input += 8;

        if (s1 >= BASE)
            s1 -= BASE;
        if (length % 0x8000 == 0)
            s2 %= BASE;
    }

    auto m_s1 = (uint16_t)s1;
    auto m_s2 = (uint16_t)s2;

    uint32_t hash = ((uint32_t)m_s2 << 16) | (uint32_t)m_s1;

    return hash;
}
