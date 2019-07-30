//
// Created by David Oberacker on 2019-07-29.
//

#include <borderlands2.hpp>

#define BOOST_LOG_DYN_LINK 1

#include <boost/date_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/asio/streambuf.hpp>

#include <openssl/sha.h>
#include <lzo/lzo2a.h>

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(lib_saveeditor_logger, boost::log::trivial::logger);

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
bool getDataFromFile(boost::filesystem::ifstream *istream,
                     uint8_t *checksum, uint64_t checksum_size,
                     uint8_t *data, uint64_t data_size) noexcept(false) {

    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    if (!istream) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "The given filestream in invalid!";
        throw std::invalid_argument("The given input stream is invalid!");
    }

    char checksum_read[20];
    (*istream).read(checksum_read, 20);
    if ((*istream).rdstate() != 0) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "The previous read operation on the stream failed!";
        if (((*istream).rdstate() & boost::filesystem::ifstream::eofbit) != 0) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "EOF was encountered while reading " << checksum_size << " bytes!";
        }
        return false;
    }

    memcpy(checksum, checksum_read, 20);

    char data_read[data_size];
    (*istream).read(data_read, data_size);
    if ((*istream).rdstate() != 0) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "The previous read operation on the stream failed!";
        if (((*istream).rdstate() & boost::filesystem::ifstream::eofbit) != 0) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "EOF was encountered while reading " << checksum_size << " bytes!";
        }
        return false;
    }

    memcpy(data, data_read, data_size);

    return true;
}

bool BORDERLANDS2_SAVE_EDITOR_API verifySave(const std::string &path) noexcept(false) {

    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug) << "Verifying savefile!";

    if (!isSaveFile(path)) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Invalid path specified";
    }

    std::unique_ptr<boost::filesystem::path> save_file = std::make_unique<boost::filesystem::path>(path);

    if (!save_file->is_absolute()) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug) << "Making specified path absolute!";
        try {
            boost::filesystem::path canonical_path = boost::filesystem::absolute(*(save_file));
            save_file.reset(nullptr);
            save_file = std::make_unique<boost::filesystem::path>(canonical_path);
        } catch (boost::filesystem::filesystem_error &ex) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
            throw std::runtime_error(ex.what());
        }
    }


    boost::filesystem::ifstream save_file_stream(*save_file);
    size_t size = boost::filesystem::file_size(*save_file) - 20;
    uint8_t checksum[20];
    uint8_t data[size];

    getDataFromFile(&save_file_stream, checksum, 20, data, size);

    save_file_stream.close();
    save_file.reset(nullptr);

    if (lzo_init() != 0) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug) << "Failed to init LZO library!";
        // TODOs: Throw error to signal library error ?
        return false;
    }

    size_t uncompressed_size = 0;
    uncompressed_size = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];

    lzo_bytep uncompressed_data = new lzo_byte[uncompressed_size];
    lzo_memset(uncompressed_data, 0, uncompressed_size);

    size_t compressed_size = size - 4;
    lzo_bytep compressed_data = new lzo_byte[compressed_size];
    lzo_memset(compressed_data, 0, compressed_size);
    lzo_memcpy(compressed_data, data + 4, compressed_size);

    // TODO: Fix lzo decompression.
    switch (lzo2a_decompress_safe(compressed_data, compressed_size, uncompressed_data, &uncompressed_size, nullptr)) {
        case LZO_E_OK:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::info)
                << "LZO decompression successful!"
                << " Failure at byte: " << uncompressed_size;
            break;
        case LZO_E_OUT_OF_MEMORY:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Out of memory!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_INPUT_OVERRUN:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Input overrun!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_OUTPUT_OVERRUN:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Output overrun!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_LOOKBEHIND_OVERRUN:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Lookbehind overrun!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_EOF_NOT_FOUND:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! EOF not found!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_INPUT_NOT_CONSUMED:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Input not consumed!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_INVALID_ARGUMENT:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Invalid Argument!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_INVALID_ALIGNMENT:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Output Alignment!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        case LZO_E_OUTPUT_NOT_CONSUMED:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed! Output not consumeds!"
                << " Failure at byte: " << uncompressed_size;
            return false;
        default:
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "LZO decompression failed!"
                << " Failure at byte: " << uncompressed_size;
            return false;
    }

    std::cout << uncompressed_data << std::endl;

    // TODO: Implement the correct separation of the save data.

    return false;
}

bool BORDERLANDS2_SAVE_EDITOR_API_NO_EXPORT isSaveFile(const std::string &path) noexcept(false) {
    boost::log::trivial::logger &logger = lib_saveeditor_logger::get();

    std::unique_ptr<boost::filesystem::path> save_file = std::make_unique<boost::filesystem::path>(path);

    if (!boost::filesystem::exists(*(save_file))) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Invalid path specified";
        return false;
    }


    if (!boost::filesystem::is_regular_file(*(save_file))) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Specified path does not lead to a file!";
        return false;
    }

    if ((save_file->extension().generic_string() != ".sav")) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Specified path does not lead to a sav file!";
        return false;
    }

    if (!save_file->is_absolute()) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::debug) << "Making specified path absolute!";
        try {
            boost::filesystem::path canonical_path = boost::filesystem::absolute(*(save_file));
            save_file.reset(nullptr);
            save_file = std::make_unique<boost::filesystem::path>(canonical_path);
        } catch (boost::filesystem::filesystem_error &ex) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error) << ex.what();
            throw std::runtime_error(ex.what());
        }
    }

    boost::filesystem::ifstream save_file_stream(*save_file);
    uint64_t size = boost::filesystem::file_size(*save_file) - 20;
    uint8_t checksum[20];
    uint8_t data[size];

    if (!getDataFromFile(&save_file_stream, checksum, 20, data, size)) {
        BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
            << "Error getting data and checksum from file: " << *save_file << "! ";

    }

    uint8_t checksum_data[20];
    SHA1(data, size, checksum_data);

    for (int i = 0; i < 20; ++i) {
        if (checksum[i] != checksum_data[i]) {
            BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::error)
                << "SHA1 checksum invalid: Byte " << i
                << " is not equal: Checksum(Data): " << std::hex << checksum_data[i] << " <-> "
                << "Checksum: " << std::hex << checksum[i];
            return false;
        }
    }

    BOOST_LOG_SEV(logger.get(), boost::log::trivial::severity_level::info)
        << "Validated save file at: " << *save_file << "! ";

    save_file_stream.close();
    save_file.reset(nullptr);

    return true;
}

