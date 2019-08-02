/*!
 * @brief This file defines a class to access and edit PathfinderKingmaker savefiles.
 *
 * @author David Oberacker
 *
 * @date 07.03.2019
 */

#pragma once

#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <rapidjson/document.h>

#include "bl_save_editor_exports.hpp"

/*!
 * @brief Namespace for Bordlerands related classes.
 */
namespace Borderlands {

    /*!
     * @brief Namespace for classes of the Borderlands::SaveEditor.
     */
    namespace SaveEditor {

        /*!
         * @brief Class representing parts of a Borderlands savefile.
         *
         * This class allows access to the player.json and party.json files from a pathfinder kingmaker savefile.
         * It allows to read and write the two files.
         *
         * @author David Oberacker
         *
         * @version 1.0.0
         *
         * @date 14.03.2019
         */
        class BORDERLANDS_SAVE_EDITOR_API SaveFile {
        private:

            /*!
             * @brief Name of the savefile that is currently edited.
             *
             * @note This can be a empty unique pointer iff no savefile has been opened.
             *
             * @warning This container should not be accessed directly.
             */
            std::unique_ptr<std::string> save_name;

            /*!
             * @brief Path to the savefile that is currently edited.
             *
             * @note This can be a empty unique pointer iff no savefile has been opened.
             *
             * @warning This container should not be accessed directly.
             */
            std::unique_ptr<boost::filesystem::path> save_file;

            /*!
             * @brief Path to the temporary copy of the savefile that is currently edited.
             *
             * @note This can be a empty unique pointer iff no savefile has been opened.
             *
             * @warning This container should not be accessed directly.
             */
            std::unique_ptr<boost::filesystem::path> temp_file;

            /*!
             * @brief Path to the savefile directory.
             *
             * @note This can be a empty unique pointer iff no savefile has been opened.
             *
             * @warning This container should not be accessed directly.
             */
            std::unique_ptr<boost::filesystem::path> output_dir;

            /*!
             * @brief JSON document loaded from the savefile containing all data regaring the player.
             *
             * @note This can be a empty unique pointer iff no savefile has been opened.
             *
             * @warning This container should not be accessed directly.
             */
            std::unique_ptr<rapidjson::Document> player_json;

            /*!
             * @brief JSON document loaded from the savefile containing all data regaring the party.
             *
             * @note This can be a empty unique pointer iff no savefile has been opened.
             *
             * @warning This container should not be accessed directly.
             */
            std::unique_ptr<rapidjson::Document> party_json;

        public:

            /*!
             * @brief Creates a new savefile instance.
             *
             * @note The new instance is uninitialized until the ::open(const std::string &save_file_path)
             *      method has been called.
             */
            SaveFile();

            /*!
             * @brief Uninitializes and deletes the instance.
             *
             * @note All temporary files and changes will be deleted and will not be saved to the disk.
             */
            virtual ~SaveFile() noexcept(false);

            /*!
             * Opens a given savefile and initliaizes the instance.
             *
             * This method has to be called before any other method of the instance should be called.
             *
             * @throw std::runtime_error Invalid properties of the found file. Origins can be,
             *      iff the file is not a zip file, or the file cannot be opend by the library.
             * @throw std::invalid_argument The given argument is invalid, meaning its not pointing to a file.
             *
             * @param save_file_path Path to a save file. If the save file is not a zks/zip file or isn't a valid
             *      PathfinderKingmaker save file, the method throws a exception.
             */
            void open(const std::string &save_file_path) noexcept(false);

            /*!
             * @brief Returns the json document describing the player form the currently loaded savefile.
             *
             * @throws std::runtime_error If there is no open savefile this exception will be called.
             *
             * @warning This can only be called after ::open(const std::string &save_file_path) has been called.
             *
             * @return A reference to the player.json document.
             */
            rapidjson::Document &getPlayerJson() noexcept(false);

            /*!
             * @brief Returns the json document describing the party form the currently loaded savefile.
             *
             * @throws std::runtime_error If there is no open savefile this exception will be called.
             *
             * @warning This can only be called after ::open(const std::string &save_file_path) has been called.
             *
             * @return A reference to the party.json document.
             */
            rapidjson::Document &getPartyJson() noexcept(false);

            /*!
             * @brief Saves the currently opened savefile in the output_dir with the given name.
             *
             * Iff there were any changes to the savefile contents these will be persisted.
             *
             * @throw std::runtime_error If there is no open savefile this exception will be called.
             *
             * @throw std::invalid_argument If there already is a savefile with the same name in the output_dir.
             *
             * @warning This can only be called after ::open(const std::string &save_file_path) has been called.
             *
             * @param output_file_name The name for the new savefile.
             */
            void save(const std::string &output_file_name) noexcept(false);

            /*!
             * @brief Saves the currently opened savefile in the output_dir with the name of the
             *          input file + the current date_time in front of it.
             *
             * Iff there were any changes to the savefile contents these will be persisted.
             *
             * @throw std::runtime_error If there is no open savefile this exception will be called.
             *
             * @throw std::invalid_argument If there already is a savefile with the same name in the output_dir.
             *
             * @warning This can only be called after ::open(const std::string &save_file_path) has been called.
             */
            void save() noexcept(false);
        };
    }
}


