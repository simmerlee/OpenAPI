#include "oa_base64.h"
#include <string.h>

static const char* base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"    // 0 ~ 25
        "abcdefghijklmnopqrstuvwxyz"    // 25 ~ 51
        "0123456789+/";                 // 52 ~ 62

static const unsigned char base64_chars_map[] = 
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     //  0 ~ 16
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     // 17 ~ 32
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,     // 33 ~ 47
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,     // 48 ~ 63
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,     // 64 ~ 79
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,     // 80 ~ 95
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,     // 96 ~ 112
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1      // 113 ~ 127
};

__inline int is_base64_char(char c)
{
    if( (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        (c == '+' || c == '/') )
        return 0;   // True
    return -1;      // False
}

int oa_base64_encode(const char* src, unsigned length, char* buf, unsigned bufSize, unsigned* outputSize)
{
    unsigned outBytes = 0;
    unsigned modifySize = 0;
    unsigned i = 0;     // src counter
    unsigned j = 0;     // buf counter
    unsigned loop = 0;  
    unsigned loopCount = 0;
    char tail[4];
    *(unsigned*)tail = 0;

    if(src == NULL || length == 0)
        return -1;

    outBytes = length / 3 * 4;
    if(length % 3 != 0)
        outBytes += 4;
    if(buf == NULL || bufSize < outBytes)
        return -1;

    switch(length % 3)
    {
    case 0: modifySize = 0; break;
    case 1: modifySize = 2; break;
    case 2: modifySize = 1; break;
    default: modifySize = 0;
    }

    loopCount = length / 3;
    while(loop < loopCount)
    {
        buf[j++] = base64_chars[(src[i] & 0xfc) >> 2];
        buf[j++] = base64_chars[((src[i] & 0x03) << 4) + ((src[i+1] & 0xf0) >> 4)];
        buf[j++] = base64_chars[((src[i+1] & 0x0f) << 2) + ((src[i+2] & 0xc0) >> 6)];
        buf[j++] = base64_chars[src[i+2] & 0x3f];
        
        i += 3;
        loop ++;
    }
    if(i < length)
    {
        loop = 0;
        while(i < length)
            tail[loop++] = src[i++];
        buf[j++] = base64_chars[(tail[0] & 0xfc) >> 2];
        buf[j++] = base64_chars[((tail[0] & 0x03) << 4) + ((tail[1] & 0xf0) >> 4)];
        buf[j++] = base64_chars[((tail[1] & 0x0f) << 2) + ((tail[2] & 0xc0) >> 6)];
        buf[j++] = base64_chars[tail[2] & 0x3f];

        for(i = 0; i < modifySize; i++)
            buf[--j] = '=';
    }

    if(outputSize != NULL)
        *outputSize = outBytes;

    return 0;
}

int oa_base64_decode(const char* src, unsigned length, char* buf, unsigned bufSize, unsigned* outputSize)
{
    unsigned outBytes = 0;
    unsigned i = 0;             // src counter
    unsigned j = 0;             // buf counter
    unsigned k = 0;             // temp counter
    unsigned tailLength = 0;    // number of '='
    unsigned loop = 0;          
    unsigned loopCount = 0;

    if(src == NULL || length == 0 || length % 4 != 0)
        return -1;
    
    for(i = length - 1; src[i] == '=' && i != 0; i--)
        tailLength ++;
    if(tailLength > 2)
        return -1;

    outBytes = length / 4 * 3 - tailLength;
    if(bufSize < outBytes)
        return -1;

    
    loopCount = length / 4;
    if(tailLength > 0)
        loopCount --;


    for(i = 0, loop = 0; loop < loopCount; loop++, i += 4)
    {
        for(k = i; k < 4; k++)
            if(is_base64_char(src[k]) != 0)
                return -1;

        buf[j++] = (base64_chars_map[(unsigned char)src[i]] << 2) 
                 + ((base64_chars_map[(unsigned char)src[i+1]] & 0x30) >> 4);
        buf[j++] = ((base64_chars_map[(unsigned char)src[i+1]] & 0xf) << 4) 
                 + ((base64_chars_map[(unsigned char)src[i+2]] & 0x3c) >> 2);
        buf[j++] = ((base64_chars_map[(unsigned char)src[i+2]] & 0x3) << 6) 
                 + base64_chars_map[(unsigned char)src[i+3]];        
    }

    if(tailLength-- > 0)
    {
        buf[j++] = (base64_chars_map[(unsigned char)src[i]] << 2) 
                 + ((base64_chars_map[(unsigned char)src[i+1]] & 0x30) >> 4);    
        if(tailLength-- > 0)            
            buf[j++] = ((base64_chars_map[(unsigned char)src[i+1]] & 0xf) << 4) 
                 + ((base64_chars_map[(unsigned char)src[i+2]] & 0x3c) >> 2);
    }

    if(outputSize != NULL)
        *outputSize = outBytes;

    return 0;
}
