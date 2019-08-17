
#pragma once

#include <bl_common_exports.hpp>

namespace D4v3 {
    /*!
     * @brief Namespace for Bordlerands related classes.
     */
    namespace Borderlands {

        /*!
         * @brief Namespace for Common classes.
         */
        namespace Common {

            /*!
             * @brief Namespace for function related o Huffman en-/decoding.
             */
            namespace Huffman {

                bool BORDERLANDS_COMMON_API decode(const char* input_array, uint32_t input_size, char* output_array, int32_t output_size) noexcept(false);
            }

            namespace Streams {

                enum BORDERLANDS_COMMON_API Endian {
                     big_endian,
                     little_endian
                };

                void BORDERLANDS_COMMON_API read_uint32(std::istream* stream, uint32_t* num, D4v3::Borderlands::Common::Streams::Endian endianess) noexcept(false);

                void BORDERLANDS_COMMON_API read_int32(std::istream* stream, int32_t* num, D4v3::Borderlands::Common::Streams::Endian endianess) noexcept(false);
            }
        }
    }
}
