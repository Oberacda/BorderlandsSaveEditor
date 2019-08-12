//
// Created by David Oberacker on 2019-07-31.
//

#include <string>
#include <map>
#include <queue>
#include <boost/dynamic_bitset.hpp>

#include "common/common.hpp"

struct Node
{
    uint8_t Symbol;
    bool IsLeaf;
    int64_t Left;
    int64_t Right;
};

uint8_t decodeByte(boost::dynamic_bitset<> data, int64_t* offset) {
    uint8_t value = 0;
    for (int i = 7; i >= 0; --i)
    {
        uint8_t v = data[(*offset)++] ? 1 : 0;
        value |= (uint8_t)(v << (unsigned int) i);
    }
    return value;
};

int decodeNode(const boost::dynamic_bitset<>& data, int64_t* offset, Node* tree, int64_t* index)
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
    auto* bitArray = new boost::dynamic_bitset<>(input_size * 8);
    convertBytesToBits(input_array, input_size, bitArray);

    auto* tree = new Node[511];
    int64_t index = 0;
    int64_t offset = 0;

    decodeNode(*bitArray, &offset, tree, &index);

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

    delete[] tree;
    delete bitArray;

    return true;
}


