#ifndef MD5_H
#define MD5_H

typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned long long uint64;

using std::string;

class md5
{
    private:
        md5();
        ~md5();
        static const uint32 sine_table[64];
        static const uint32 shift_table[4][4];
        static const char hex_table[16];
        static uint32* pad_input(const char *input, uint64 input_length, uint32 *block_count);
        static uint32 rotate_left(uint32 number, uint32 times);
        static byte* digest_message(uint32 *padded_input, uint32 block_count);
        static string to_string(byte* digest);
    public:
        static string create_hash(const string &input_string);
};

#endif // MD5_H
