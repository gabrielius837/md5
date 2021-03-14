#include <cstring>
#include <string>
#include "md5.h"

const uint32 md5::sine_table[64] =
{
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

const uint32 md5::shift_table[4][4] =
{
    { 7, 12, 17, 22 },
    { 5,  9, 14, 20 },
    { 4, 11, 16, 23 },
    { 6, 10, 15, 21 }
};

const char md5::hex_table[16] =
{
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};

uint32* md5::pad_input(const char* input, uint64 input_length, uint32* block_count)
{
    // amount of bytes in last block
    uint32 count = input_length % 64;
    // amount of bytes needed for padding
    uint64 padding_length = count < 56 ? (56 - count) : (120 - count);
    // adjust output size for input length storage
    uint32 output_length = (input_length + padding_length + 8);
    // initialize output
    byte* output = new byte[output_length];
    // copy input to output
    memcpy(output, input, input_length);
    // append 1 bit right after input
    output[input_length] = 0x80;
    // append 0's
    for (uint32 i = input_length + 1; i < output_length - 8; i++)
       output[i] = 0;
    // append length of the input after padding
    uint64 input_bit_length = input_length * 8;
    memcpy(output + output_length - 8, &input_bit_length, 8);
    // assign count of blocks
    *block_count = output_length / 64;
    return (uint32*)output;
}

uint32 md5::rotate_left(uint32 number, uint32 times)
{
    // bit shifts number n times to the left without losing it's bits hence rotate
    return ( number << times ) | ( number >> ( 32 - times ));
}

string md5::to_string(byte* digest)
{
    string hash;
    hash.reserve(32);

    for (uint32 i = 0; i < 16; i++)
    {
        int a = digest[i] / 16;
        int b = digest[i] % 16;
        hash.push_back(md5::hex_table[a]);
        hash.push_back(md5::hex_table[b]);
    }

    return hash;
}

byte* md5::digest_message(uint32 *padded_input, uint32 block_count)
{
    uint32  h0 = 0x67452301,
            h1 = 0xefcdab89,
            h2 = 0x98badcfe,
            h3 = 0x10325476;

    uint32  a = 0,
            b = 0,
            c = 0,
            d = 0,
            i = 0,
            f = 0,
            g = 0,
            temp = 0;

    for (uint32 block = 0; block < block_count; block++)
    {
        a = h0;
        b = h1;
        c = h2;
        d = h3;

        for (i = 0; i < 64; i++)
        {
            uint32 option = i / 16;
            switch (option)
            {
                // 0 - 15
                case 0:
                {
                    f = (b & c) | ((~b) & d);
                    g = i;
                    break;
                }
                // 16 - 31
                case 1:
                {
                    f = (d & b) | ((~d) & c);
                    g = (5 * i + 1) % 16;
                    break;
                }
                // 32 - 47
                case 2:
                {
                    f = b ^ c ^ d;
                    g = (3 * i + 5) % 16;
                    break;
                }
                // 48 - 63
                case 3:
                {
                    f = c ^ (b | (~d));
                    g = (7 * i) % 16;
                    break;
                }
            }

            temp = d;
            d = c;
            c = b;
            b = b + md5::rotate_left((a + f + md5::sine_table[i] + padded_input[block * 16 + g]), md5::shift_table[option][i % 4]);
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }

    uint32* result = new uint32[4];

    result[0] = h0;
    result[1] = h1;
    result[2] = h2;
    result[3] = h3;

    return (byte*)result;
}

string md5::create_hash(const string &input_string)
{
    const char* input = input_string.c_str();
    uint64 input_length = input_string.size();
    uint32 block_count = 0;

    uint32* padded_input = md5::pad_input(input, input_length, &block_count);
    byte* digest = md5::digest_message(padded_input, block_count);
    string output = md5::to_string(digest);

    delete[] padded_input;
    delete[] digest;

    return output;
}
