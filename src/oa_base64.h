#ifndef _OPENAPI_UTILITY_BASE64_H_
#define _OPENAPI_UTILITY_BASE64_H_

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

int oa_base64_encode(const char* src, unsigned length, char* buf, unsigned bufSize, unsigned* outputSize);
int oa_base64_decode(const char* src, unsigned length, char* buf, unsigned bufSize, unsigned* outputSize);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_UTILITY_BASE64_H_
