#ifndef _OPENAPI_UTILITY_STRING_H_
#define _OPENAPI_UTILITY_STRING_H_

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

typedef struct
{
    char* data;
    unsigned int len;

    // flowing member SHOULD NOT be modified manually!
    unsigned int capacity;
}oa_string_t;

int oa_string_init(oa_string_t* str);
int oa_string_init_by_cstr(oa_string_t* str, const char* cstr);
int oa_string_destroy(oa_string_t* str);
int oa_string_assign(oa_string_t* dst, const oa_string_t* src);
int oa_string_assign_cstr(oa_string_t* dst, const char* src);
int oa_string_assign_n(oa_string_t* dst, const char* src, unsigned int count);
int oa_string_append(oa_string_t* dst, const oa_string_t* src);
int oa_string_append_cstr(oa_string_t* dst, const char* src);
int oa_string_append_n(oa_string_t* dst, const char* src, unsigned int count);
int oa_string_append_char(oa_string_t* dst, char ch);
int oa_string_find(oa_string_t* str, const oa_string_t* partten, char** pos);
int oa_string_find_cstr(oa_string_t* str, const char* partten, char** pos);
int oa_string_expand(oa_string_t* str, unsigned int new_capacity);
int oa_string_clear(oa_string_t* str);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_UTILITY_STRING_H_
