/**
 * @file        save_file.hpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save file management class.
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
#include "save_data.hpp"

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

    /*!
     * @brief Class to manage a single borderlands 2 save file.
     *
     * Every instance of this class manages a single save file mapped to a path on the disk.
     * New files are required to have a path assigned to them.
     *
     * @author David Oberacker
     */
    class BORDERLANDS2_SAVE_LIB_API Borderlands2_Save_File {
    private:

        /*!
         * Pointer to the instance manageing the save file data contained in the save file.
         */
        std::unique_ptr<Borderlands2_Save_Data> saveDataPtr;

        std::unique_ptr<std::filesystem::path> saveFilePathPtr;

        /*!
         * @brief Decompresses a minilzo compressed data array of length compressed_size into
         * an uncompressed array of length uncompressed_size.
         *
         * @param[in] compressed_data The compressed data array of length compressed_size.
         * This array is required to be at least compressed_size bytes long.
         * @param compressed_size The size of the compressed data array.
         * @param[out] uncompressed_data The resulting uncompressed data array of length uncompressed_size.
         * @param uncompressed_size The size of the uncompressed data array. The decoded data has to be exactly
         * this length.
         */
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



    public:
        explicit Borderlands2_Save_File() noexcept(false);

        /*!
         * @brief Function to verify a Borderlands2 save file.
         *
         * @throws std::runtime_error If there is a filesystem error this exception is thrown.
         *
         * @return true Iff the save file is valid.
         * @return false Iff the save file is invalid.
         */
        bool verifySave() noexcept(false);

        /*!
         * @brief Loads a save file at the given path.
         *
         * @throw std::runtime_error If there is a filesystem error this exception is thrown.
         * @throw std::invalid_argument If the given input path is not accessible this exception is thrown.
         *
         * @param path The loacation of the save file to load.
         */
        void loadSaveFile(const std::string &path) noexcept(false);

        /*!
         * @brief Checks if two save file instances are not equal.
         *
         * Two equal save file instances can not be interchanged for each other without unexpected differences.
         *
         * @param rhs The instance ot check if not equal.
         * @return true iff they can not subsitute each other.
         */
        bool operator==(const Borderlands2_Save_File &rhs) const;

        /*!
         * @brief Checks if two save file instances are not equal.
         *
         * Two equal save file instances can not be interchanged for each other without unexpected differences.
         *
         * @param rhs The instance ot check if not equal.
         * @return true iff they can not subsitute each other.
         */
        bool operator!=(const Borderlands2_Save_File &rhs) const;
    };


}


#endif //BORDERLANDS2_SAVE_LIB_BORDERLANDS2_HPP
