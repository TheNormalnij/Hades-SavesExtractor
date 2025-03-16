// adler32.h - originally written and placed in the public domain by Wei Dai

/// \file adler32.h
/// \brief Class file for ADLER-32 checksum calculations


#pragma once

#include <cstddef>
#include <cstdint>

namespace Adler32 {
uint32_t Calculate(const uint8_t *input, size_t length);
}
