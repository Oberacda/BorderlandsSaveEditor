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
#include "bl2_save_lib_exports.hpp"

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
             * @brief Checks if the file at a specific path is a valid Borderlands2 save file.
             *
             * @details This method checks if the file is a regular file with the '.sav' extension and the
             *  SHA1 checksum at the beginning of the file is correct.
             *
             *  @throws std::runtime_error If there is a filesystem error this exception is thrown.
             *
             * @param path The input path to check the file at.
             * @return true iff the file is a save file, else false.
             */
            bool BORDERLANDS2_SAVE_LIB_API_NO_EXPORT isSaveFile(const std::string &path) noexcept(false);

            /**
             * Function to verify a Borderlands2 save file.
             *
             * @throws std::runtime_error If there is a filesystem error this exception is thrown.
             *
             * @param path Input path to the save file. This has to be a valid file path in the current filesystem.
             *  Escaped characters have to be escaped in the string.
             *
             * @return true Iff the save file is valid.
             * @return false Iff the save file is invalid.
             */
            bool BORDERLANDS2_SAVE_LIB_API verifySave(const std::string &path)  noexcept(false);

            /**
             * @brief Dumps the content of a Borderlands2 savefile as a json file to the specified output path.
             *
             * @param in_path Input path to the save file. This has to be a valid file path in the current filesystem.
             *  Escaped characters have to be escaped in the string. This has to point to a file.
             * @param out_path Output path to the json file. This has to be a valid file path in the current filesystem.
             *  Escaped characters have to be escaped in the string. This has to point to a file.
             */
            void BORDERLANDS2_SAVE_LIB_API dumpSaveJson(const std::string& in_path, const std::string& out_path)  noexcept(false);
        }




#endif //BORDERLANDS2_SAVE_LIB_BORDERLANDS2_HPP
