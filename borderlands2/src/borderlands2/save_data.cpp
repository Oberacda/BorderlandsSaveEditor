/**
 * @file        save_data.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save data management class.
 * @details     Implementation of a class to manage a borderlands2 save data.
 * @version     0.1
 * @date        2019-11-01
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include "borderlands2/save_data.hpp"

namespace D4v3::Borderlands::Borderlands2 {

    Borderlands2_Save_Data::Borderlands2_Save_Data(std::unique_ptr<WillowTwoPlayerSaveGame>& saveGamePtr) {
        this->saveGamePtr.swap(saveGamePtr);

    }

    bool Borderlands2_Save_Data::operator==(const Borderlands2_Save_Data &rhs) const {
        return saveGamePtr == rhs.saveGamePtr;
    }

    bool Borderlands2_Save_Data::operator!=(const Borderlands2_Save_Data &rhs) const {
        return !(rhs == *this);
    }

    Borderlands2_Save_Data::Borderlands2_Save_Data() {
        auto willowTwoPlayerSaveGamePtr = std::make_unique<WillowTwoPlayerSaveGame>();
        this->saveGamePtr.swap(willowTwoPlayerSaveGamePtr);
    }
}