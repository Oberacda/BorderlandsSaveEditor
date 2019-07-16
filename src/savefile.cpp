/*!
 * @brief This file implements a class to access and edit PathfinderKingmaker savefiles.
 *
 * This implementation uses libZip and zlib as well as boost::log and boost::filesystem for the implementation.
 *
 * @author David Oberacker
 *
 * @date 07.03.2019
 */

#include "savefiles.hpp"

#define BOOST_LOG_DYN_LINK 1

#include <exception>
#include <ostream>

#include <boost/date_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>

#include <zip.h>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>

static const char *player_file = "player.json";
static const char *party_file = "party.json";

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(lib_saveeditor_logger, boost::log::trivial::logger);

PathfinderKingmaker::SaveEditor::SaveFile::SaveFile() {

    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug) << "initlializing savefile instance!";

    this->player_json = std::unique_ptr<rapidjson::Document>();
    this->party_json = std::unique_ptr<rapidjson::Document>();
    this->output_dir = std::unique_ptr<boost::filesystem::path>();
    this->temp_file = std::unique_ptr<boost::filesystem::path>();;
    this->save_file = std::unique_ptr<boost::filesystem::path>();;
    this->save_name = std::unique_ptr<std::string>();;
}

PathfinderKingmaker::SaveEditor::SaveFile::~SaveFile() noexcept(false) {

    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::trace) << "deinitlializing savefile instance!";

    this->player_json.reset(nullptr);
    this->party_json.reset(nullptr);
    this->output_dir.reset(nullptr);
    if (this->temp_file) {
        try {
            boost::filesystem::remove(*(this->temp_file));
        } catch (boost::filesystem::filesystem_error &ex) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
            throw std::runtime_error(ex.what());
        }
    }
    this->temp_file.reset(nullptr);
    this->save_file.reset(nullptr);
    this->save_name.reset(nullptr);
}

void PathfinderKingmaker::SaveEditor::SaveFile::open(const std::string &save_file_path) noexcept(false) {

    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();
    this->save_file = std::make_unique<boost::filesystem::path>(save_file_path);

    if (!boost::filesystem::exists(*(this->save_file))) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Invalid path specified";
        this->save_file.reset(nullptr);
        throw std::invalid_argument("Not a valid path!");
    }


    if (!boost::filesystem::is_regular_file(*(this->save_file))) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Specified path does not lead to a file!";
        this->save_file.reset(nullptr);
        throw std::invalid_argument("Not a valid path to a file!");
    }


    if (!this->save_file->is_absolute()) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug) << "Making specified path absolute!";
        try {
            boost::filesystem::path canonical_path = boost::filesystem::absolute(*(this->save_file));
            this->save_file.reset(nullptr);
            this->save_file = std::make_unique<boost::filesystem::path>(canonical_path);
        } catch (boost::filesystem::filesystem_error &ex) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
            throw std::runtime_error(ex.what());
        }
    }

    if ((this->save_file->extension().generic_string() != ".zks") &&
        (this->save_file->extension().generic_string() != ".zip")) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Specified path does not lead to a zip/zks file!";
        this->save_file.reset(nullptr);
        throw std::invalid_argument("Not a valid path to a zks file!");
    }

    this->output_dir = std::make_unique<boost::filesystem::path>(this->save_file->parent_path());

    this->save_name = std::make_unique<std::string>(this->save_file->filename().generic_string());

    try {
        boost::filesystem::copy_file(*(this->save_file),
                                     boost::filesystem::temp_directory_path() / this->save_file->filename(),
                                     boost::filesystem::copy_option::overwrite_if_exists);
    } catch (boost::filesystem::filesystem_error &ex) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
        throw std::runtime_error(ex.what());
    }

    this->temp_file = std::make_unique<boost::filesystem::path>(
            boost::filesystem::temp_directory_path() / this->save_file->filename());

    BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug)
        << "Coping tempfile to - " << this->temp_file->generic_string();

    zip *zip_archive;
    zip_error_t zip_error_t;
    int zip_error_int = 0;
    struct zip_stat zip_stats{};
    zip_file *current_zip_file;

    zip_stat_init(&zip_stats);

    if (nullptr == (zip_archive = zip_open(this->temp_file->string().data(), 0, &zip_error_int))) {
        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    //Search for the file of given name

    if (0 != zip_stat(zip_archive, player_file, 0, &zip_stats)) {
        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        if (0 != zip_close(zip_archive)) {

            this->output_dir.reset(nullptr);
            if (this->temp_file) {
                try {
                    boost::filesystem::remove(*(this->temp_file));
                } catch (boost::filesystem::filesystem_error &ex) {
                    BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                    throw std::runtime_error(ex.what());
                }
            }
            this->temp_file.reset(nullptr);
            this->save_file.reset(nullptr);
            this->save_name.reset(nullptr);
            this->player_json.reset(nullptr);
            this->party_json.reset(nullptr);

            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);
            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    //Alloc memory for its uncompressed contents
    char *player_file_contents = new char[zip_stats.size + 1];
	player_file_contents[zip_stats.size] = '\0';

    //Read the compressed file
    if (nullptr == (current_zip_file = zip_fopen(zip_archive, player_file, 0))) {
        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        if (0 != zip_fclose(current_zip_file)) {
            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);

            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        if (0 != zip_close(zip_archive)) {
            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);

            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        delete[] player_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    if (-1 == zip_fread(current_zip_file, player_file_contents, zip_stats.size)) {
        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        if (0 != zip_fclose(current_zip_file)) {
            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);

            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        if (0 != zip_close(zip_archive)) {
            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);

            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        delete[] player_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    if (0 != zip_fclose(current_zip_file)) {
        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        if (0 != zip_close(zip_archive)) {
            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);

            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        delete[] player_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    this->player_json = std::make_unique<rapidjson::Document>();
    if (this->player_json->Parse(player_file_contents).HasParseError()) {

        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "player.json file not valid - Offset -"
            << this->player_json->GetErrorOffset()
            << " - "
            << rapidjson::GetParseError_En(this->player_json->GetParseError());

        if (0 != zip_close(zip_archive)) {
            zip_error_init_with_code(&zip_error_t, zip_error_int);
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "Zip handling error - " << zip_error_strerror(&zip_error_t);

            throw std::runtime_error(zip_error_strerror(&zip_error_t));
        }

        delete[] player_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);

        throw std::runtime_error("player.json file not valid!");
    }

    delete[] player_file_contents;

    if (0 != zip_stat(zip_archive, party_file, 0, &zip_stats)) {
        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    char *party_file_contents = new char[zip_stats.size + 1];
	party_file_contents[zip_stats.size] = '\0';

    //Read the compressed file
    if (nullptr == (current_zip_file = zip_fopen(zip_archive, party_file, 0))) {

        delete[] party_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);

        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    if (-1 == zip_fread(current_zip_file, party_file_contents, zip_stats.size)) {
        delete[] party_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    if (0 != zip_fclose(current_zip_file)) {
        delete[] party_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    this->party_json = std::make_unique<rapidjson::Document>();
    if (this->party_json->Parse(party_file_contents).HasParseError()) {

        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "pary.json file not valid - Offset -"
            << this->party_json->GetErrorOffset()
            << " - "
            << rapidjson::GetParseError_En(this->party_json->GetParseError());

        delete[] party_file_contents;

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);
        this->party_json.reset(nullptr);

        throw std::runtime_error("party.json file not valid!");
    }

    delete[] party_file_contents;

    //And close the archive
    if (0 != zip_close(zip_archive)) {

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);
        this->party_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }
}

rapidjson::Document &PathfinderKingmaker::SaveEditor::SaveFile::getPartyJson() noexcept(false) {
    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    if (this->party_json) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::trace)
            << "Access to the party.json file !";
        return *(this->party_json);
    } else {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Invalid state, the party.json file is null!";
        throw std::runtime_error("Invalid state, the party.json file is null!");
    }
}

rapidjson::Document &PathfinderKingmaker::SaveEditor::SaveFile::getPlayerJson() noexcept(false) {
    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    if (this->player_json) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::trace)
            << "Access to the player.json file !";
        return *(this->player_json);
    } else {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Invalid state, the player.json file is null!";
        throw std::runtime_error("Invalid state, the player.json file is null!");
    }
}

void PathfinderKingmaker::SaveEditor::SaveFile::save() noexcept(false) {


    boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();

    std::ostringstream ofstream;

    ofstream
            << timeLocal.date().year()
            << "-"
            << timeLocal.date().month()
            << "-"
            << timeLocal.date().day();
    ofstream
            << "_"
            << timeLocal.time_of_day().hours()
            << "-"
            << timeLocal.time_of_day().minutes()
            << "-"
            << timeLocal.time_of_day().seconds()
            << "___";

    boost::filesystem::path output_path(ofstream.str());
    output_path += this->save_file->filename();

    this->save(output_path.generic_string());
}

void PathfinderKingmaker::SaveEditor::SaveFile::save(const std::string &output_file_name) noexcept(false) {
    zip *zip_archive;
    zip_error_t zip_error_t;
    int zip_error_int = 0;
    zip_source *current_zip_source;

    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    if (nullptr == (zip_archive = zip_open(this->temp_file->string().data(), 0, &zip_error_int))) {

        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);
        this->party_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    rapidjson::StringBuffer string_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> party_writer(string_buffer);
    this->party_json->Accept(party_writer);

    const void *output = reinterpret_cast<const void *>(string_buffer.GetString());

    current_zip_source = zip_source_buffer_create(output, string_buffer.GetSize(), 0, &zip_error_t);

    if (-1 == zip_file_add(zip_archive, party_file, current_zip_source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8)) {
        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);
        this->party_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    string_buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> player_writer(string_buffer);
    this->player_json->Accept(player_writer);

    output = reinterpret_cast<const void *>(string_buffer.GetString());

    current_zip_source = zip_source_buffer_create(output, string_buffer.GetSize(), 0, &zip_error_t);

    if (-1 == zip_file_add(zip_archive, player_file, current_zip_source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8)) {
        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);
        this->party_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    if (0 != zip_close(zip_archive)) {
        this->output_dir.reset(nullptr);
        if (this->temp_file) {
            try {
                boost::filesystem::remove(*(this->temp_file));
            } catch (boost::filesystem::filesystem_error &ex) {
                BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
                throw std::runtime_error(ex.what());
            }
        }
        this->temp_file.reset(nullptr);
        this->save_file.reset(nullptr);
        this->save_name.reset(nullptr);
        this->player_json.reset(nullptr);
        this->party_json.reset(nullptr);

        zip_error_init_with_code(&zip_error_t, zip_error_int);
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Zip handling error - " << zip_error_strerror(&zip_error_t);
        throw std::runtime_error(zip_error_strerror(&zip_error_t));
    }

    try {
        boost::filesystem::copy_file(*(this->temp_file),
                                     (*this->output_dir) / boost::filesystem::path(output_file_name).filename(),
                                     boost::filesystem::copy_option::fail_if_exists);
    } catch (boost::filesystem::filesystem_error &ex) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
        throw std::invalid_argument(ex.what());
    }

}


