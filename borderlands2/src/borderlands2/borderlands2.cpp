/**
 * @file        borderlands2.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save management functions.
 * @details     Implementation of functions to load, verify and dump data from and to Borderlands 2 save files.
 * @version     0.1
 * @date        2019-08-26
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */
#include "bl2_save_lib_exports.hpp"
#include "borderlands2/borderlands2.hpp"
#include "borderlands2/WillowTwoPlayerSaveGame.pb.h"

#include <common/common.hpp>

#include <boost/interprocess/streams/bufferstream.hpp>

#include <openssl/sha.h>

#include <minilzo-2.10/minilzo.h>

#include <google/protobuf/util/json_util.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <istream>
#include <sstream>

namespace D4v3::Borderlands::Borderlands2 {
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
    void BORDERLANDS2_SAVE_LIB_API_NO_EXPORT getDataFromFile(std::ifstream *istream,
                                                             uint8_t *checksum, uint64_t checksum_size,
                                                             uint8_t *data, uint64_t data_size) noexcept(false) {
        if (!istream->is_open()) {
            throw std::invalid_argument("The given input stream is invalid!");
        }

        char* checksum_read = new char[20];
        memset(checksum_read, 0 , 20);

        (*istream).read(checksum_read, 20);
        if ((*istream).rdstate() != 0) {
            std::ostringstream string_stream;
            string_stream << "Data access from file failed! ";
            if (((*istream).rdstate() & std::ifstream::eofbit) != 0) {
                string_stream << "EOF was encountered while reading " << checksum_size << " bytes!";
            }
            throw std::runtime_error(string_stream.str());
        }

        memcpy(checksum, checksum_read, 20);
        delete [] checksum_read;
        checksum_read = nullptr;

        char* data_read = new char[data_size];
        memset(data_read, 0 , data_size);

        (*istream).read(data_read, data_size);
        int32_t istream_state = (int32_t) (*istream).rdstate();

        if (istream_state != 0) {
            std::ostringstream string_stream;
            string_stream
                    << "Failed to read: " << data_size << " bytes! Could only read: " << (*istream).gcount() << " bytes!";
            if ((istream_state & std::ifstream::eofbit) != 0) {
                string_stream
                        << "EOF was encountered!";
            }
            if ((istream_state & std::ifstream::badbit) != 0) {
                string_stream
                        << "BadBit was encountered!";
            }
            if ((istream_state & std::ifstream::failbit) != 0) {
                string_stream
                        << "FailBit was encountered!";
            }
            throw std::runtime_error(string_stream.str());
        }

        memcpy(data, data_read, data_size);
        delete[] data_read;
        data_read = nullptr;
    }

    void BORDERLANDS2_SAVE_LIB_API_NO_EXPORT loadSave(const std::string& path,
                                                      std::unique_ptr<WillowTwoPlayerSaveGame>& save_game) noexcept(false) {
        std::ostringstream exception_message_stream;

        if(!save_game) {
            throw std::invalid_argument("The given pointer to the save game is invalid!");
        }

        if (!D4v3::Borderlands::Borderlands2::isSaveFile(path)) {
            throw std::invalid_argument("Specified path is invalid! It does not point to a "
                                        "valid save file or the save file ist corrupted!");
        }

        std::unique_ptr<std::filesystem::path> save_file = std::make_unique<std::filesystem::path>(path);

        if (!save_file->is_absolute()) {
            try {
                std::filesystem::path canonical_path = std::filesystem::absolute(*(save_file));
                save_file.reset(nullptr);
                save_file = std::make_unique<std::filesystem::path>(canonical_path);
            } catch (std::filesystem::filesystem_error &ex) {
                throw std::runtime_error(ex.what());
            }
        }


        std::ifstream save_file_stream(*save_file, std::ifstream::in | std::ifstream::binary);
        size_t size = std::filesystem::file_size(*save_file) - 20;
        auto* checksum = new uint8_t[20];
        memset(checksum, 0, 20);

        auto* data = new uint8_t[size];
        memset(data, 0, size);

        try {
            getDataFromFile(&save_file_stream, checksum, 20, data, size);
        } catch (std::runtime_error& exc) {
            exception_message_stream
                    << "Read access to the given file failes! "
                    << exc.what();
            throw std::runtime_error(exception_message_stream.str());
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
                break;
            case LZO_E_OUT_OF_MEMORY:
                exception_message_stream
                        << "LZO decompression failed! Out of memory!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_INPUT_OVERRUN:
                exception_message_stream
                        << "LZO decompression failed! Input overrun!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_OUTPUT_OVERRUN:
                exception_message_stream
                        << "LZO decompression failed! Output overrun!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_LOOKBEHIND_OVERRUN:
                exception_message_stream
                        << "LZO decompression failed! Lookbehind overrun!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_EOF_NOT_FOUND:
                exception_message_stream
                        << "LZO decompression failed! EOF not found!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_INPUT_NOT_CONSUMED:
                exception_message_stream
                        << "LZO decompression failed! Input not consumed!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_INVALID_ARGUMENT:
                exception_message_stream
                        << "LZO decompression failed! Invalid Argument!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_INVALID_ALIGNMENT:
                exception_message_stream
                        << "LZO decompression failed! Output Alignment!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            case LZO_E_OUTPUT_NOT_CONSUMED:
                exception_message_stream
                        << "LZO decompression failed! Output not consumeds!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
            default:
                exception_message_stream
                        << "LZO decompression failed!"
                        << " Failure at byte: " << uncompressed_size;
                throw std::runtime_error(exception_message_stream.str());
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

        int innerCompressedSize = D4v3::Borderlands::Common::Util::uint32_to_int32(innerSize) - 3 - 4 - 4 - 4;

        char* innerCompressedBytes = new char [innerCompressedSize];
        memset(innerCompressedBytes, 0, innerCompressedSize);
        input_stream.read(innerCompressedBytes, innerCompressedSize);

        delete[] uncompressed_data;
        uncompressed_data = nullptr;
        uncompressed_data_char = nullptr;

        char* innerUncompressedBytes = new char[innerUncompressedSize];
        memset(innerUncompressedBytes, 0, innerUncompressedSize);

        if(!D4v3::Borderlands::Common::Huffman::decode(innerCompressedBytes, innerCompressedSize, innerUncompressedBytes, innerUncompressedSize)) {
            exception_message_stream
                    << "Huffman decoding failed!";
            throw std::runtime_error(exception_message_stream.str());
        };

        if(!(*save_game).ParseFromArray(innerUncompressedBytes, innerUncompressedSize)) {
            exception_message_stream
                    << "Protobuf deserialization failed!";
            throw std::runtime_error(exception_message_stream.str());
        }
    }

}


bool BORDERLANDS2_SAVE_LIB_API D4v3::Borderlands::Borderlands2::verifySave(const std::string &path) noexcept(false) {

    auto save_game = std::make_unique<WillowTwoPlayerSaveGame>();

    loadSave(path, save_game);

    return true;
}

bool BORDERLANDS2_SAVE_LIB_API_NO_EXPORT D4v3::Borderlands::Borderlands2::isSaveFile(const std::string &path) noexcept(false) {
    std::ostringstream exception_message_stream;
    std::unique_ptr<std::filesystem::path> save_file = std::make_unique<std::filesystem::path>(path);

    if (!std::filesystem::exists(*(save_file))) {
        exception_message_stream
            << "Invalid path specified";
        return false;
    }


    if (!std::filesystem::is_regular_file(*(save_file))) {
        return false;
    }

    if ((save_file->extension().generic_string() != ".sav")) {
        return false;
    }

    if (!save_file->is_absolute()) {
        try {
            std::filesystem::path canonical_path = std::filesystem::absolute(*(save_file));
            save_file.reset(nullptr);
            save_file = std::make_unique<std::filesystem::path>(canonical_path);
        } catch (std::filesystem::filesystem_error &ex) {
            throw std::runtime_error(ex.what());
        }
    }

    std::ifstream save_file_stream(*save_file, std::ifstream::in | std::ifstream::binary);

    uint64_t size = std::filesystem::file_size(*save_file) - 20;

    auto* checksum = new uint8_t[20];
    memset(checksum, 0, 20);

    auto* data = new uint8_t[size];
    memset(data, 0, size);

    try {
        getDataFromFile(&save_file_stream, checksum, 20, data, size);
    } catch (std::runtime_error& exc) {
        exception_message_stream
                << "Error getting data and checksum from file: " << *save_file << "! "
                << exc.what();
        return false;
    }

    auto* checksum_data = new uint8_t[20];
    memset(checksum_data, 0, 20);

    SHA1(data, size, checksum_data);

    for (int i = 0; i < 20; ++i) {
        if (checksum[i] != checksum_data[i]) {
            exception_message_stream
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

    save_file_stream.close();
    save_file.reset(nullptr);

    return true;
}

void BORDERLANDS2_SAVE_LIB_API
D4v3::Borderlands::Borderlands2::dumpSaveJson(const std::string& in_path, const std::string& out_path) noexcept(false) {
    auto out_save_file = std::make_unique<std::filesystem::path>(out_path);

    if (std::filesystem::exists(*(out_save_file))) {
        throw std::invalid_argument("Specified path is invalid! It does point to a valid file!");
    }
    if (!out_save_file->is_absolute()) {
        try {
            std::filesystem::path canonical_path = std::filesystem::absolute(*(out_save_file));
            out_save_file.reset(nullptr);
            out_save_file = std::make_unique<std::filesystem::path>(canonical_path);
        } catch (std::filesystem::filesystem_error &ex) {
            throw std::runtime_error(ex.what());
        }
    }

    auto save_game = std::make_unique<WillowTwoPlayerSaveGame>();

    loadSave(in_path, save_game);

    auto* json_string = new std::string();
    google::protobuf::util::JsonPrintOptions print_options;
    print_options.always_print_primitive_fields = true;
    print_options.add_whitespace = true;

    google::protobuf::util::MessageToJsonString(*save_game, json_string, print_options);

    std::ofstream out_save_stream((*out_save_file), std::ofstream::out);
    out_save_stream << *json_string;
    out_save_stream.close();

    save_game.reset(nullptr);
    delete json_string;
}

