/**
 * @file        common.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Implementation of common stream operations.
 * @version     0.1
 * @date        2019-08-26
 * 
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>

#include <cstring>

#include "common/common.hpp"

void BORDERLANDS_COMMON_API D4v3::Borderlands::Common::Streams::read_uint32(std::istream *stream, uint32_t *num,
                                                                            D4v3::Borderlands::Common::Streams::Endian endianess) noexcept(false) {
    (*num) = 0;
    char number_array[4];
    memset(number_array, 0, 4);
    (*stream).read(number_array, 4);

    if (endianess == big_endian) {
        (*num) = (uint32_t) (((uint8_t) number_array[3]) & ((uint8_t) 0xFF));
        (*num) |= (uint32_t) (((uint8_t) number_array[2]) & ((uint8_t) 0xFF)) << 8;
        (*num) |= (uint32_t) (((uint8_t) number_array[1]) & ((uint8_t) 0xFF)) << 16;
        (*num) |= (uint32_t) (((uint8_t) number_array[0]) & ((uint8_t) 0xFF)) << 24;
    } else if (endianess == little_endian) {
        (*num) = (uint32_t) (((uint8_t) number_array[3]) & ((uint8_t) 0xFF)) << 24;
        (*num) |= (uint32_t) (((uint8_t) number_array[2]) & ((uint8_t) 0xFF)) << 16;
        (*num) |= (uint32_t) (((uint8_t) number_array[1]) & ((uint8_t) 0xFF)) << 8;
        (*num) |= (uint32_t) (((uint8_t) number_array[0]) & ((uint8_t) 0xFF));
    }
}

void BORDERLANDS_COMMON_API D4v3::Borderlands::Common::Streams::read_int32(std::istream *stream, int32_t *num,
                                                                            D4v3::Borderlands::Common::Streams::Endian endianess) noexcept(false) {
    (*num) = 0;
    char number_array[4];
    memset(number_array, 0, 4);
    (*stream).read(number_array, 4);

    if (endianess == D4v3::Borderlands::Common::Streams::big_endian) {
        (*num) = (int32_t) (((uint8_t) number_array[3]) & ((uint8_t) 0xFF));
        (*num) |= (int32_t) (((uint8_t) number_array[2]) & ((uint8_t) 0xFF)) << 8;
        (*num) |= (int32_t) (((uint8_t) number_array[1]) & ((uint8_t) 0xFF)) << 16;
        (*num) |= (int32_t) (((uint8_t) number_array[0]) & ((uint8_t) 0xFF)) << 24;
    } else if (endianess == D4v3::Borderlands::Common::Streams::little_endian) {
        (*num) = (int32_t) (((uint8_t) number_array[3]) & ((uint8_t) 0xFF)) << 24;
        (*num) |= (int32_t) (((uint8_t) number_array[2]) & ((uint8_t) 0xFF)) << 16;
        (*num) |= (int32_t) (((uint8_t) number_array[1]) & ((uint8_t) 0xFF)) << 8;
        (*num) |= (int32_t) (((uint8_t) number_array[0]) & ((uint8_t) 0xFF));
    }
}


int32_t BORDERLANDS_COMMON_API D4v3::Borderlands::Common::Util::uint32_to_int32(uint32_t x) noexcept(false) {
    if (x <= INT32_MAX)
        return static_cast<int>(x);
    if (x >= INT32_MAX)
        return static_cast<int>(x - INT32_MIN) + INT32_MIN;
    throw std::runtime_error("Invalid uint32 to int32 conversion!"); // Or whatever else you like
}
