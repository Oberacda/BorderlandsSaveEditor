/**
 * @file        save_file.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save file management functions.
 * @details     Implementation of functions to load, verify and dump data from and to Borderlands 2 save files.
 * @version     0.1
 * @date        2019-08-26
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */
#include "borderlands2/save_file.hpp"

#include <common/common.hpp>

#include <boost/interprocess/streams/bufferstream.hpp>

#include <openssl/sha.h>

#include <minilzo-2.10/minilzo.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <istream>
#include <sstream>

namespace D4v3::Borderlands::Borderlands2 {
    void Borderlands2_Save_File::getDataFromFile(std::ifstream *istream, uint8_t *checksum, uint64_t checksum_size,
                                                 uint8_t *data, uint64_t data_size) noexcept(false) {
        if (!istream->is_open()) {
            throw std::invalid_argument("The given input stream is invalid!");
        }

        auto *checksum_char = reinterpret_cast<char *>(checksum);
        (*istream).read(checksum_char, checksum_size);
        if ((*istream).rdstate() != 0) {
            std::ostringstream string_stream;
            string_stream << "Data access from file failed! ";
            if (((*istream).rdstate() & std::ifstream::eofbit) != 0) {
                string_stream << "EOF was encountered while reading " << checksum_size << " bytes!";
            }
            throw std::runtime_error(string_stream.str());
        }

        auto *data_char = reinterpret_cast<char *>(data);
        (*istream).read(data_char, data_size);
        auto istream_state = (int32_t) (*istream).rdstate();

        if (istream_state != 0) {
            std::ostringstream string_stream;
            string_stream
                    << "Failed to read: " << data_size << " bytes! Could only read: " << (*istream).gcount()
                    << " bytes!";
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
    }

    bool Borderlands2_Save_File::verifySave() noexcept(false) {
        //TODO: Implement function.
        return true;
    }

    void Borderlands2_Save_File::decompress_lzo(unsigned char *compressed_data, size_t compressed_size,
                                                unsigned char *uncompressed_data,
                                                size_t uncompressed_size) noexcept(false) {
        std::ostringstream exception_message_stream;
        switch (lzo1x_decompress_safe(compressed_data, compressed_size, uncompressed_data, &uncompressed_size,
                                      nullptr)) {
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
    }

    bool Borderlands2_Save_File::isSaveFile(const std::string &path) noexcept(false) {
        std::ostringstream exception_message_stream;
        std::filesystem::path save_file(path);

        if (!save_file.is_absolute()) {
            try {
                save_file = std::filesystem::absolute(save_file);
            } catch (std::filesystem::filesystem_error &ex) {
                throw std::runtime_error(ex.what());
            }
        }

        if (!std::filesystem::exists(save_file)) {
            exception_message_stream
                    << "Invalid path specified";
            return false;
        }


        if (!std::filesystem::is_regular_file(save_file)) {
            return false;
        }

        if ((save_file.extension().generic_string() != ".sav")) {
            return false;
        }


        std::ifstream save_file_stream(save_file, std::ifstream::in | std::ifstream::binary);

        uint64_t size = std::filesystem::file_size(save_file) - 20;

        auto checksum_ptr = std::make_unique<uint8_t[]>(20);
        memset(checksum_ptr.get(), 0, 20);

        auto data_ptr = std::make_unique<uint8_t[]>(size);
        memset(data_ptr.get(), 0, size);

        try {
            getDataFromFile(&save_file_stream, checksum_ptr.get(), 20, data_ptr.get(), size);
        } catch (std::runtime_error &exc) {
            exception_message_stream
                    << "Error getting data and checksum from file: " << save_file << "! "
                    << exc.what();
            return false;
        }

        auto checksum_data_ptr = std::make_unique<uint8_t[]>(20);
        memset(checksum_data_ptr.get(), 0, 20);

        SHA1(data_ptr.get(), size, checksum_data_ptr.get());

        for (int i = 0; i < 20; ++i) {
            if (checksum_ptr[i] != checksum_data_ptr[i]) {
                exception_message_stream
                        << "SHA1 checksum invalid: Byte " << i
                        << " is not equal: Checksum(Data): " << std::hex << checksum_data_ptr[i] << " <-> "
                        << "Checksum: " << std::hex << checksum_ptr[i];
                return false;
            }
        }

        data_ptr.reset(nullptr);
        checksum_ptr.reset(nullptr);
        checksum_data_ptr.reset(nullptr);

        save_file_stream.close();

        return true;
    }

    Borderlands2_Save_File::Borderlands2_Save_File() noexcept(false) {
        this->saveFilePathPtr = std::make_unique<std::filesystem::path>();
        this->saveDataPtr = std::make_unique<Borderlands2_Save_Data>();
    }

    void
    Borderlands2_Save_File::loadSaveFile(const std::string &path) noexcept(false) {
        std::ostringstream exception_message_stream;

        if (this->saveDataPtr == nullptr) {
            throw std::invalid_argument("The given pointer to the save game is invalid!");
        }

        if (!D4v3::Borderlands::Borderlands2::Borderlands2_Save_File::isSaveFile(path)) {
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
        auto checksum_ptr = std::make_unique<uint8_t[]>(20);
        memset(checksum_ptr.get(), 0, 20);

        auto data_ptr = std::make_unique<uint8_t[]>(size);
        memset(data_ptr.get(), 0, size);

        try {
            getDataFromFile(&save_file_stream, checksum_ptr.get(), 20, data_ptr.get(), size);
        } catch (std::runtime_error &exc) {
            exception_message_stream
                    << "Read access to the given file failes! "
                    << exc.what();
            throw std::runtime_error(exception_message_stream.str());
        }

        save_file_stream.close();

        checksum_ptr.reset(nullptr);

        size_t uncompressed_size = 0;
        uncompressed_size = data_ptr[0] << 24 | data_ptr[1] << 16 | data_ptr[2] << 8 | data_ptr[3];

        auto uncompressed_data_ptr = std::make_unique<unsigned char[]>(uncompressed_size);
        memset(uncompressed_data_ptr.get(), 0, uncompressed_size);

        size_t compressed_size = size - 4;
        auto compressed_data_ptr = std::make_unique<unsigned char[]>(compressed_size);
        memset(compressed_data_ptr.get(), 0, compressed_size);
        memcpy(compressed_data_ptr.get(), data_ptr.get() + 4, compressed_size);

        data_ptr.reset(nullptr);

        decompress_lzo(compressed_data_ptr.get(), compressed_size, uncompressed_data_ptr.get(), uncompressed_size);

        compressed_data_ptr.reset(nullptr);

        auto *uncompressed_char_data = reinterpret_cast<char *>(uncompressed_data_ptr.release());
        auto uncompressed_char_data_ptr = std::unique_ptr<char[]>(uncompressed_char_data);
        uncompressed_char_data = nullptr;

        boost::interprocess::bufferstream input_stream(uncompressed_char_data_ptr.get(), uncompressed_size);


        uint32_t innerSize = 0;
        D4v3::Borderlands::Common::Streams::read_uint32(&input_stream, &innerSize,
                                                        D4v3::Borderlands::Common::Streams::Endian::big_endian);

        char magic_number[3];
        memset(magic_number, 0, 3);
        input_stream.read(magic_number, 3);

        uint32_t version = 0;
        D4v3::Borderlands::Common::Streams::read_uint32(&input_stream, &version,
                                                        D4v3::Borderlands::Common::Streams::Endian::little_endian);

        D4v3::Borderlands::Common::Streams::Endian endianess;
        if (version != 2) {
            endianess = D4v3::Borderlands::Common::Streams::Endian::big_endian;
        } else {
            endianess = D4v3::Borderlands::Common::Streams::Endian::little_endian;
        }

        uint32_t hash;
        D4v3::Borderlands::Common::Streams::read_uint32(&input_stream, &hash, endianess);

        int32_t inner_uncompressed_size = 0;
        D4v3::Borderlands::Common::Streams::read_int32(&input_stream, &inner_uncompressed_size, endianess);

        int inner_compressed_size = D4v3::Borderlands::Common::Util::uint32_to_int32(innerSize) - 3 - 4 - 4 - 4;

        auto inner_compressed_bytes_ptr = std::make_unique<char[]>(inner_compressed_size);
        memset(inner_compressed_bytes_ptr.get(), 0, inner_compressed_size);
        input_stream.read(inner_compressed_bytes_ptr.get(), inner_compressed_size);

        uncompressed_char_data_ptr.reset(nullptr);

        auto inner_uncompressed_bytes_ptr = std::make_unique<char[]>(inner_uncompressed_size);
        memset(inner_uncompressed_bytes_ptr.get(), 0, inner_uncompressed_size);

        if (!D4v3::Borderlands::Common::Huffman::decode(inner_compressed_bytes_ptr.get(), inner_compressed_size,
                                                        inner_uncompressed_bytes_ptr.get(), inner_uncompressed_size)) {
            inner_compressed_bytes_ptr.reset(nullptr);

            inner_uncompressed_bytes_ptr.reset(nullptr);

            exception_message_stream
                    << "Huffman decoding failed!";
            throw std::runtime_error(exception_message_stream.str());
        };
        inner_compressed_bytes_ptr.reset(nullptr);

        auto saveFilePtr = std::make_unique<WillowTwoPlayerSaveGame>();

        if (!(saveFilePtr->ParseFromArray(inner_uncompressed_bytes_ptr.get(), inner_uncompressed_size))) {
            inner_uncompressed_bytes_ptr.reset(nullptr);

            exception_message_stream
                    << "Protobuf deserialization failed!";
            throw std::runtime_error(exception_message_stream.str());
        }
        inner_uncompressed_bytes_ptr.reset(nullptr);

        auto newSaveDataPtr = std::make_unique<Borderlands2_Save_Data>(saveFilePtr);
        this->saveDataPtr.swap(newSaveDataPtr);

        newSaveDataPtr.reset(nullptr);

        this->saveFilePathPtr.swap(save_file);
        save_file.reset(nullptr);
    }

    bool Borderlands2_Save_File::operator==(const Borderlands2_Save_File &rhs) const {
        return saveDataPtr == rhs.saveDataPtr &&
               saveFilePathPtr == rhs.saveFilePathPtr;
    }

    bool Borderlands2_Save_File::operator!=(const Borderlands2_Save_File &rhs) const {
        return !(rhs == *this);
    }
}







