/**
 * @file        common.hpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Common Borderlands save management functions.
 * @details     Utility and other common functions used in different borderlands save management
 *              functions. These functions allow a safe and flexilble management for certain
 *              operations.
 * @version     0.1
 * @date        2019-08-26
 * 
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#ifndef BORDERLANDS_COMMON_H
#define BORDERLANDS_COMMON_H

#pragma once

#include <cstdint>
#include <fstream>

#include <bl_common_exports.hpp>

/**
 * @brief Namespace for projects by D4v3.
 */
namespace D4v3 {
    
    /**
     * @brief Namespace for Bordlerands related classes.
     */
    namespace Borderlands {

        /**
         * @brief Namespace for Common classes.
         */
        namespace Common {

            /*!
             * @brief Namespace for function related to Huffman en-/decoding.
             */
            namespace Huffman {
                
                /*!
                 * @brief Function to decode a huffman encoded array.
                 * 
                 * @param[in] input_array   The array containing the encoded data. This array has to be at least
                 *                          input_size elements.
                 * @param[in] input_size    Lenght of the input array. Has to be greater than 0.
                 * @param[in] output_array  The array to put the decoded data into. Has to be preallocated with
                 *                          at least output_size elements.
                 * @param[in] output_size   Output array size. Has to be greater than 0 and big enough to fit the
                 *                          decoded data.
                 * @return true             If the decompression was successful.
                 * @return false            Decompression failed an the output array might be populated partly.
                 */
                bool BORDERLANDS_COMMON_API decode(const char* input_array, uint32_t input_size, char* output_array, int32_t output_size) noexcept(false);
            }

            /*!
            * @brief Namespace for function related to stream handeling
            */
            namespace Streams {

                ///Enum for stream endianess.
                enum Endian {

                     ///Big endian encoding.
                     big_endian,
                     ///Little endian encoding.
                     little_endian
                };
                /*!
                 * @brief Read a single unsigned 32-bit integer from a std::istream.
                 * 
                 * @param[in] stream    The stream to read the data from.
                 * @param[out] num      The number read from the stream. 
                 *                      This has to be a pointer to a preallocated unsigned integer.
                 * @param[in] endianess The endianess of the data stored in the stream.
                 */
                void BORDERLANDS_COMMON_API read_uint32(std::istream* stream, uint32_t* num, D4v3::Borderlands::Common::Streams::Endian endianess) noexcept(false);

                /*!
                 * @brief Read a single signed 32-bit integer from a std::istream.
                 * 
                 * @param[in] stream    The stream to read the data from.
                 * @param[out] num      The number read from the stream. 
                 *                      This has to be a pointer to a preallocated signed integer.
                 * @param[in] endianess The endianess of the data stored in the stream.
                 */
                void BORDERLANDS_COMMON_API read_int32(std::istream* stream, int32_t* num, D4v3::Borderlands::Common::Streams::Endian endianess) noexcept(false);
            }
        }
    }
}

#endif /* BORDERLANDS_COMMON_H */
