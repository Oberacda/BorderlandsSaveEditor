/**
 * @file        decoder.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Implementation of huffman decoder operations.
 * @version     0.1
 * @date        2019-08-26
 * 
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include <string>
#include <map>
#include <queue>

#include <cstdlib>

#include <boost/dynamic_bitset.hpp>

#include "common/common.hpp"

/**
 * @brief Struct describing a node in a huffman tree.
 */
struct Node
{
    /**
     * @brief Symbol encoded in this node of the tree.
     */
    uint8_t Symbol;

    /**
     * @brief Is this node a leaf?
     */
    bool IsLeaf;

    /**
     * @brief Index of the left subtree. If node is a leaf, this is -1.
     */
    int64_t Left;

    /**
     * @brief Index of the right subtree. If node is a leaf, this is -1.
     */
    int64_t Right;
};

/**
 * @brief Decodes a byte with a offset from a boost::dynamic_bitset.
 * 
 * @param data The bitset to use.
 * @param offset The offset to use.
 * @return uint8_t The byte that was read for the bitset.
 */
uint8_t decodeByte(boost::dynamic_bitset<> data, int64_t* offset) {
    uint8_t value = 0;
    for (int i = 7; i >= 0; --i)
    {
        uint8_t v = data[(*offset)++] ? 1 : 0;
        value |= (uint8_t)(v << (unsigned int) i);
    }
    return value;
};

/**
 * @brief Function to decode a node in a encoded huffman tree.
 * 
 * @param[in] data Bitset containing the encoded data.
 * @param[in] offset The offset from where to start decoding.
 * @param[out] tree The generated tree, the decoded data should be added to.
 * @param[out] index The index where to start adding the decoded nodes to.
 * @return int64_t The decoded node index.
 */
int64_t decodeNode(const boost::dynamic_bitset<>& data, int64_t* offset, Node* tree, int64_t* index)
{
    uint32_t current = (*index);
    (*index)++;

    bool isLeaf = data[(*offset)++];

    if (isLeaf)
    {
        uint8_t value = decodeByte(data, offset);
        tree[current].Left = -1;
        tree[current].Right = -1;
        tree[current].IsLeaf = true;
        tree[current].Symbol = value;
    }
    else
    {
        tree[current].Left = decodeNode(data, offset, tree, index);
        tree[current].Right = decodeNode(data, offset, tree, index);
        tree[current].IsLeaf = false;
    }

    return current;
}

/**
 * @brief Function to convert a byte sequence to a bitset.
 * 
 * @param[in] data The input byte sequence that should be converted.
 * @param data_size The length of the input byte sequence. This should not be 0.
 * @param[out] bit_map A pointer to the bitmap the results should be stored in.
 */
 void convertBytesToBits(const char* data, uint32_t data_size, boost::dynamic_bitset<>* bit_map)
{
    for (int i = 0, o = 0; i < data_size; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            bool s = ((data[i] & (1 << j)) != 0);
            (*bit_map)[o++] = s;
        }
    }
}

bool BORDERLANDS_COMMON_API
D4v3::Borderlands::Common::Huffman::decode(const char *input_array, uint32_t input_size, char *output_array,
                                           int32_t output_size) noexcept(false) {
    auto bitArray = std::make_unique<boost::dynamic_bitset<>>(input_size * 8);
    convertBytesToBits(input_array, input_size, bitArray.get());

    auto tree = std::make_unique<Node []>(511);
    int64_t index = 0;
    int64_t offset = 0;
    decodeNode(*bitArray, &offset, tree.get(), &index);

    int32_t left = output_size;

    uint32_t o = 0;
    while (left > 0)
    {
        Node branch = tree[0];
        while (!branch.IsLeaf)
        {
            branch = tree[(*bitArray)[offset++] == false ? branch.Left : branch.Right];
        }

        output_array[o++] = branch.Symbol;
        left--;
    }

    return true;
}

