#ifndef _OPENAPI_UTILITY_CRC_H_
#define _OPENAPI_UTILITY_CRC_H_

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

int ou_crc32(const char* src, unsigned length, unsigned *result);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_UTILITY_CRC_H_
