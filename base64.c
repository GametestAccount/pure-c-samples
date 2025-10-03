#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static inline char from_base64_table(char n) //ASCII character to base64 number
{
    if ((n >= 0x41) && (n <= 0x5A)) // "A-Z"
        return n - 65;
    else if ((n >= 0x61) && (n <= 0x7A)) // "a-z"
        return n - 71;
    else if ((n >= 0x30) && (n <= 0x39)) // "0-1"
        return n + 4;
    else if (n == 0x2B) // "+"
        return 62;
    else if (n == 0x2F) // "/"
        return 63;
    else
        return 0;
}

extern char *base64_encode(const uint8_t *bytes, size_t input_len, size_t *output_len)
{
    if (!bytes || !input_len)
        return NULL;
    
    size_t size = ceil(input_len / 3.0) * 4;
    
    char *buffer = malloc(size + 1);
    
    if (!buffer)
        return NULL;
    
    if (output_len)
        *output_len = size;
    
    for (size_t i = 0, j = 0; i < size; i += 4, j += 3)
    {
        switch ((size == (i + 4)) ? (input_len % 3) : 0) //if this is the last iteration
        {
            case 0: //if there are 3 bytes
                buffer[i] = base64_table[bytes[j] >> 2];
                buffer[i + 1] = base64_table[((bytes[j] & 0x03) << 4) | (bytes[j + 1] >> 4)];
                buffer[i + 2] = base64_table[((bytes[j + 1] & 0x0F) << 2) | (bytes[j + 2] >> 6)];
                buffer[i + 3] = base64_table[bytes[j + 2] & 0x3F];
                break;
            case 1: //if there is 1 byte
                buffer[i] = base64_table[bytes[j] >> 2];
                buffer[i + 1] = base64_table[(bytes[j] & 0x03) << 4];
                buffer[i + 2] = '=';
                buffer[i + 3] = '=';
                break;
            case 2: //if there are 2 bytes
                buffer[i] = base64_table[bytes[j] >> 2];
                buffer[i + 1] = base64_table[((bytes[j] & 0x03) << 4) | (bytes[j + 1] >> 4)];
                buffer[i + 2] = base64_table[(bytes[j + 1] & 0x0F) << 2];
                buffer[i + 3] = '=';
                break;
        }
    }
    
    buffer[size] = '\0';
    
    return buffer;
}

extern uint8_t *base64_decode(const char *data, size_t input_len, size_t *output_len)
{
    if (!data)
        return NULL;
    
    if (!input_len)
        input_len = strlen(data);
    
    if ((input_len % 4) != 0)
        return NULL;
    
    int r = 0;
    
    if (data[input_len - 1] == '=')
        r++;
    if (data[input_len - 2] == '=')
        r++;
    
    size_t size = (input_len / 4) * 3 - r;
    
    uint8_t *buffer = malloc(size);
    
    if (!buffer)
        return NULL;
    
    if (output_len)
        *output_len = size;
    
    for (size_t i = 0, j = 0; i < input_len; i += 4, j += 3)
    {
        char a = from_base64_table(data[i]);
        char b = from_base64_table(data[i + 1]);
        char c = from_base64_table(data[i + 2]);
        char d = from_base64_table(data[i + 3]);
        
        switch ((input_len == (i + 4)) ? r : 0) //if this is the last iteration
        {
            case 0: //if there are 3 bytes
                buffer[j] = (a << 2) | (b >> 4);
                buffer[j + 1] = (b << 4) | (c >> 2);
                buffer[j + 2] = (c << 6) | d;
                break;
            case 1: //if there is 1 byte
                buffer[j] = (a << 2) | (b >> 4);
                buffer[j + 1] = (b << 4) | (c >> 2);
                break;
            case 2: //if there are 2 bytes
                buffer[j] = (a << 2) | (b >> 4);
                break;
        }
    }
    
    return buffer;
}