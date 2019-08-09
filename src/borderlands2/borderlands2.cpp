//
// Created by David Oberacker on 2019-07-29.
//

#include "borderlands2/borderlands2.hpp"

#define BOOST_LOG_DYN_LINK 1

#include <boost/date_time.hpp>

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/interprocess/streams/bufferstream.hpp>

#include <openssl/sha.h>
#include <minilzo-2.10/minilzo.h>

#include <common/common.hpp>
#include <borderlands2/WillowTwoPlayerSaveGame.pb.h>
#include <borderlands2/borderlands2_logger.hpp>

//BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(lib_saveeditor_logger, boost::log::trivial::logger);
//BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(lib_saveeditor_logger, boost::log::sources::severity_logger_mt< >,  (keywords::severity = error)(keywords::channel = "borderlands2"))

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

    // boost::log::trivial::logger &logger = lib_saveeditor_logger::get();
    if (!istream->is_open()) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "The given filestream in invalid!";
        throw std::invalid_argument("The given input stream is invalid!");
    }

    char* checksum_read = new char[20];
    memset(checksum_read, 0 , 20);

    (*istream).read(checksum_read, 20);
    if ((*istream).rdstate() != 0) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "The previous read operation on the stream failed!";
        if (((*istream).rdstate() & boost::filesystem::ifstream::eofbit) != 0) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "EOF was encountered while reading " << checksum_size << " bytes!";
        }
        return false;
    }

    memcpy(checksum, checksum_read, 20);
    delete [] checksum_read;
    checksum_read = nullptr;

    char* data_read = new char[data_size];
    memset(data_read, 0 , data_size);

    (*istream).read(data_read, data_size);
    int32_t istream_state = (int32_t) (*istream).rdstate();

    if (istream_state != 0) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Failed to read: " << data_size << " bytes! Could only read: " << (*istream).gcount() << " bytes!";
        if ((istream_state & boost::filesystem::ifstream::eofbit) != 0) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "EOF was encountered!";
        }
        if ((istream_state & boost::filesystem::ifstream::badbit) != 0) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "BadBit was encountered!";
        }
        if ((istream_state & boost::filesystem::ifstream::failbit) != 0) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "FailBit was encountered!";
        }
        return false;
    }

    memcpy(data, data_read, data_size);
    delete[] data_read;
    data_read = nullptr;

    return true;
}

bool BORDERLANDS2_SAVE_EDITOR_API verifySave(const std::string &path) noexcept(false) {

    // boost::log::trivial::logger &logger = lib_saveeditor_logger::get();
    BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::debug) << "Verifying savefile!";

    if (!isSaveFile(path)) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Invalid path specified";
        return false;
    }

    std::unique_ptr<boost::filesystem::path> save_file = std::make_unique<boost::filesystem::path>(path);

    if (!save_file->is_absolute()) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::debug) << "Making specified path absolute!";
        try {
            boost::filesystem::path canonical_path = boost::filesystem::absolute(*(save_file));
            save_file.reset(nullptr);
            save_file = std::make_unique<boost::filesystem::path>(canonical_path);
        } catch (boost::filesystem::filesystem_error &ex) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error) << ex.what();
            throw std::runtime_error(ex.what());
        }
    }


    boost::filesystem::ifstream save_file_stream(*save_file, boost::filesystem::ifstream::in | boost::filesystem::ifstream::binary);
    size_t size = boost::filesystem::file_size(*save_file) - 20;
    auto* checksum = new uint8_t[20];
    memset(checksum, 0, 20);

    auto* data = new uint8_t[size];
    memset(data, 0, size);

    if(!getDataFromFile(&save_file_stream, checksum, 20, data, size)) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error) << "Could not get data from file!";
        return false;
    }

    save_file_stream.close();
    save_file.reset(nullptr);

    delete[] checksum;
    checksum = nullptr;

    size_t uncompressed_size = 0;
    uncompressed_size = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];

    auto* uncompressed_data = new unsigned char[uncompressed_size];
    memset(uncompressed_data, 0, uncompressed_size);

    size_t compressed_size = size - 4;
    auto* compressed_data = new unsigned char[compressed_size];
    memset(compressed_data, 0, compressed_size);
    memcpy(compressed_data, data + 4, compressed_size);

    delete[] data;
    data = nullptr;

    switch (lzo1x_decompress_safe(compressed_data, compressed_size, uncompressed_data, &uncompressed_size, nullptr)) {
        case LZO_E_OK:
            BOOST_LOG_CHANNEL_SEV(D4v3::Borderlands2::borderlands2_logger::get(), "Decompression", D4v3::Borderlands2::severity_level::info)
                << boost::log::add_value("Uncompressed size(bytes)", uncompressed_size)
                << "LZO decompression successful!";
            break;
        case LZO_E_OUT_OF_MEMORY:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Out of memory!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_INPUT_OVERRUN:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Input overrun!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_OUTPUT_OVERRUN:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Output overrun!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_LOOKBEHIND_OVERRUN:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Lookbehind overrun!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_EOF_NOT_FOUND:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! EOF not found!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_INPUT_NOT_CONSUMED:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Input not consumed!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_INVALID_ARGUMENT:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Invalid Argument!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_INVALID_ALIGNMENT:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Output Alignment!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        case LZO_E_OUTPUT_NOT_CONSUMED:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed! Output not consumed!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
        default:
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "LZO decompression failed!"
                << boost::log::add_value("Bytes consumed", uncompressed_size);
            return false;
    }


    //Freeing compressed data space.
    delete[] compressed_data;
    compressed_data = nullptr;

    auto* uncompressed_data_char = reinterpret_cast<char *>(uncompressed_data);


    boost::interprocess::bufferstream input_stream(uncompressed_data_char, uncompressed_size);

    uint32_t innerSize = 0;
    D4v3::Borderlands::Common::Streams::read_uint32(&input_stream, &innerSize, D4v3::Borderlands::Common::Streams::Endian::big_endian);

    char magicNumber[3];
    memset(magicNumber, 0 ,3);
    input_stream.read(magicNumber, 3);

    uint32_t version = 0;
    D4v3::Borderlands::Common::Streams::read_uint32(&input_stream, &version, D4v3::Borderlands::Common::Streams::Endian::little_endian);

    D4v3::Borderlands::Common::Streams::Endian endianess;
    if (version != 2) {
        endianess = D4v3::Borderlands::Common::Streams::Endian::big_endian;
    } else {
        endianess = D4v3::Borderlands::Common::Streams::Endian::little_endian;
    }

    uint32_t hash;
    D4v3::Borderlands::Common::Streams::read_uint32(&input_stream, &hash, endianess);

    int32_t innerUncompressedSize = 0;
    D4v3::Borderlands::Common::Streams::read_int32(&input_stream, &innerUncompressedSize, endianess);

    int innerCompressedSize = innerSize - 3 - 4 - 4 - 4;
    char* innerCompressedBytes = new char [innerCompressedSize];
    memset(innerCompressedBytes, 0, innerCompressedSize);
    input_stream.read(innerCompressedBytes, innerCompressedSize);

    delete[] uncompressed_data;
    uncompressed_data = nullptr;
    uncompressed_data_char = nullptr;

    char* innerUncompressedBytes = new char[innerUncompressedSize];
    memset(innerUncompressedBytes, 0, innerUncompressedSize);

    D4v3::Borderlands::Common::Huffman::decode(innerCompressedBytes, innerCompressedSize, innerUncompressedBytes, innerUncompressedSize);

    WillowTwoPlayerSaveGame save_game;
    if(!save_game.ParseFromArray(innerUncompressedBytes, innerUncompressedSize)) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Deserialization failed!";
        return false;
    }

    std::cout << save_game.playerclass() << std::endl;

    // TODO: Implement the correct separation of the save data.
    return true;
}

bool BORDERLANDS2_SAVE_EDITOR_API_NO_EXPORT isSaveFile(const std::string &path) noexcept(false) {
    std::unique_ptr<boost::filesystem::path> save_file = std::make_unique<boost::filesystem::path>(path);

    if (!boost::filesystem::exists(*(save_file))) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Invalid path specified";
        return false;
    }


    if (!boost::filesystem::is_regular_file(*(save_file))) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Specified path does not lead to a file!";
        return false;
    }

    if ((save_file->extension().generic_string() != ".sav")) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Specified path does not lead to a sav file!";
        return false;
    }

    if (!save_file->is_absolute()) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::debug) << "Making specified path absolute!";
        try {
            boost::filesystem::path canonical_path = boost::filesystem::absolute(*(save_file));
            save_file.reset(nullptr);
            save_file = std::make_unique<boost::filesystem::path>(canonical_path);
        } catch (boost::filesystem::filesystem_error &ex) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error) << ex.what();
            throw std::runtime_error(ex.what());
        }
    }

    BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::debug) << "Save file path: " << *save_file;

    boost::filesystem::ifstream save_file_stream(*save_file, boost::filesystem::ifstream::in | boost::filesystem::ifstream::binary);

    uint64_t size = boost::filesystem::file_size(*save_file) - 20;

    auto* checksum = new uint8_t[20];
    memset(checksum, 0, 20);

    auto* data = new uint8_t[size];
    memset(data, 0, size);

    if (!getDataFromFile(&save_file_stream, checksum, 20, data, size)) {
        BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
            << "Error getting data and checksum from file: " << *save_file << "! ";
        return false;
    }

    auto* checksum_data = new uint8_t[20];
    memset(checksum_data, 0, 20);

    SHA1(data, size, checksum_data);

    for (int i = 0; i < 20; ++i) {
        if (checksum[i] != checksum_data[i]) {
            BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::error)
                << "SHA1 checksum invalid: Byte " << i
                << " is not equal: Checksum(Data): " << std::hex << checksum_data[i] << " <-> "
                << "Checksum: " << std::hex << checksum[i];
            return false;
        }
    }

    delete[] data;
    data = nullptr;
    delete[] checksum;
    checksum = nullptr;
    delete[] checksum_data;
    checksum_data = nullptr;

    BOOST_LOG_SEV(D4v3::Borderlands2::borderlands2_logger::get(), D4v3::Borderlands2::severity_level::info)
        << "Validated save file at: " << *save_file << "! ";

    save_file_stream.close();
    save_file.reset(nullptr);

    return true;
}

