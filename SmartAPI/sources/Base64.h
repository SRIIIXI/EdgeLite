#ifndef _BASE_64
#define _BASE_64

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "StringEx.h"

class Base64
{
public:
    Base64();
    void encodeBase64(const unsigned char *data,  size_t input_length,  unsigned long &output_length, std::string &encodedString);
    unsigned char *decodeBase64(const char *data,  size_t input_length,  unsigned long &output_length);
private:
    void deallocateDecodingTable();
};

#endif
