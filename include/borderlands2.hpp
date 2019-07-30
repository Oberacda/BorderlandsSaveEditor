//
// Created by David Oberacker on 2019-07-29.
//

#ifndef BORDERLANDSSAVEEDITOR_BORDERLANDS2_HPP
#define BORDERLANDSSAVEEDITOR_BORDERLANDS2_HPP

#pragma once

#include <string>
#include "bl2_save_editor_exports.hpp"

/*!
 * @brief Checks if the file at a specific path is a valid Borderlands2 save file.
 *
 * @details This method checks if the file is a regular file with the '.sav' extension and the
 *  SHA1 checksum at the beginning of the file is correct.
 *
 * @param path The input path to check the file at.
 * @return true iff the file is a save file, else false.
 */
bool BORDERLANDS2_SAVE_EDITOR_API_NO_EXPORT isSaveFile(const std::string &path) noexcept(false);

bool BORDERLANDS2_SAVE_EDITOR_API verifySave(const std::string &path)  noexcept(false);


#endif //BORDERLANDSSAVEEDITOR_BORDERLANDS2_HPP