/**
 * @file        borderlands2.hpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save management functions.
 * @details     Functions to load, verify and dump data from and to Borderlands 2 save files.
 * @version     0.1
 * @date        2019-08-26
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#ifndef BORDERLANDS2_SAVE_LIB_BORDERLANDS2_HPP
#define BORDERLANDS2_SAVE_LIB_BORDERLANDS2_HPP

#pragma once

#include <string>
#include <filesystem>

#include "bl2_save_lib_exports.hpp"
#include "WillowTwoPlayerSaveGame.pb.h"

/**
 * @namespace D4v3
 * @brief Namespace for projects by D4v3.
 */

/**
 * @namespace D4v3::Borderlands
 * @brief Namespace for Borderlands related classes.
 */

/**
 * @namespace D4v3::Borderlands::Borderlands2
 * @brief Namespace for Borderlands2 related classes.
 */

namespace D4v3::Borderlands::Borderlands2 {

            class BORDERLANDS2_SAVE_LIB_API Borderlands2_Save_File {
            private:
                std::unique_ptr<WillowTwoPlayerSaveGame> playerSaveGamePtr;

                static void decompress_lzo(
                        unsigned char *compressed_data,
                        size_t compressed_size,
                        unsigned char *uncompressed_data,
                        size_t uncompressed_size
                ) noexcept(false);

                /*!
                 * @brief Gets the data and the checksum from a input file stream.
                 *
                 * The checksum are always the first checksum_size bytes of the file and data are the data_size bytes following.
                 * @param[in] istream The file stream to read from! If this is invalid the function throws a error.
                 * @param[out] checksum A pointer to the start of a array that will contain the checksum. The array has to be preallocated with size == checksum_size.
                 * @param[in] checksum_size The size of the checksum. For SHA sums this should be 20.
                 * @param[out] data A pointer to the start of a array that will contain the data. The array has to be preallocated with size == data_size.
                 * @param[in] data_size The size of the data.
                 *
                 * @throw std::invalid_argument If the given input stream is not accessible this exception is thrown.
                 *
                 * @return true on success, else otherwise
                 */
                static void getDataFromFile(
                        std::ifstream *istream,
                        uint8_t *checksum,
                        uint64_t checksum_size,
                        uint8_t *data,
                        uint64_t data_size
                ) noexcept(false);

                /*!
                 * @brief Checks if the file at a specific path is a valid Borderlands2 save file.
                 *
                 * @details This method checks if the file is a regular file with the '.sav' extension and the
                 *  SHA1 checksum at the beginning of the file is correct.
                 *
                 * @throws std::runtime_error If there is a filesystem error this exception is thrown.
                 *
                 * @param path The input path to check the file at.
                 * @return true iff the file is a save file, else false.
                 */
                static bool isSaveFile(const std::string &path) noexcept(false);

                static void loadSaveFile(const std::string &path, WillowTwoPlayerSaveGame *save_game) noexcept(false);

            public:
                Borderlands2_Save_File(const std::string &path) noexcept(false);

                [[nodiscard]] WillowTwoPlayerSaveGame *const getPlayerSaveGamePtr() const;

                /**
                 * Function to verify a Borderlands2 save file.
                 *
                 * @throws std::runtime_error If there is a filesystem error this exception is thrown.
                 *
                 * @return true Iff the save file is valid.
                 * @return false Iff the save file is invalid.
                 */
                bool verifySave() noexcept(false);

                /**
                 * @brief Dumps the content of a Borderlands2 savefile as a json file to the specified output path.
                 *
                 * @param out_path Output path to the json file. This has to be a valid file path in the current filesystem.
                 *  Escaped characters have to be escaped in the string. This has to point to a file.
                 */
                void dumpSave(const std::string& out_path) noexcept(false);

                bool operator==(const Borderlands2_Save_File &rhs) const;

                bool operator!=(const Borderlands2_Save_File &rhs) const;
            };
        }




#endif //BORDERLANDS2_SAVE_LIB_BORDERLANDS2_HPP
