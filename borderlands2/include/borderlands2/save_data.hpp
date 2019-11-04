/**
 * @file        save_data.hpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save data management class.
 * @details     Class to manage a borderlands2 save data.
 * @version     0.1
 * @date        2019-11-01
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#ifndef BORDERLANDS2_SAVE_LIB_BORDERLANDS2_SAVE_DATA_HPP
#define BORDERLANDS2_SAVE_LIB_BORDERLANDS2_SAVE_DATA_HPP

#pragma once

#include <string>
#include <filesystem>

#include "bl2_save_lib_exports.hpp"
#include "WillowTwoPlayerSaveGame.pb.h"


namespace D4v3::Borderlands::Borderlands2 {

    /*!
     * @brief Class to manage a single borderlands 2 save game data.
     *
     * @author David Oberacker
     */
    class BORDERLANDS2_SAVE_LIB_API Borderlands2_Save_Data {
    private:
        /*!
         * Pointer saving the underlying save game data in protobuf format.
         */
        std::unique_ptr<WillowTwoPlayerSaveGame> saveGamePtr = std::make_unique<WillowTwoPlayerSaveGame>();

    public:

        /*!
         * @brief Creates a new instance of the game data from a existing protobuf save data description.
         *
         * @param saveGamePtr Pointer to a existing protobuf save data.
         */
        explicit Borderlands2_Save_Data(std::unique_ptr<WillowTwoPlayerSaveGame>& saveGamePtr);

        /*!
         * @brief Creates a new instance with default save data values.
         */
        Borderlands2_Save_Data();

        /*!
         * @brief Checks if two save data instances are equal.
         *
         * Two equal save file instances can be interchanged for each other without unexpected differences.
         *
         * @param rhs The instance ot check if equal.
         * @return true iff they can subsitute each other.
         */
        bool operator==(const Borderlands2_Save_Data &rhs) const;

        /*!
         * @brief Checks if two save data instances are not equal.
         *
         * Two equal save file instances can not be interchanged for each other without unexpected differences.
         *
         * @param rhs The instance ot check if not equal.
         * @return true iff they can not subsitute each other.
         */
        bool operator!=(const Borderlands2_Save_Data &rhs) const;
    };

}

#endif //BORDERLANDS2_SAVE_LIB_BORDERLANDS2_SAVE_DATA_HPP
