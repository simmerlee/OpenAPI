#include "oa_string.h"
#include <stdlib.h>
#include <string.h>

#define OU_STRING_DEFAULT_CAPACITY              16
#define OU_STRING_EXPAND_FACTOR                 1.5

int oa_string_init(oa_string_t* str)
{
    str->len = 0;
    str->data = (char*)malloc(sizeof(char) * OU_STRING_DEFAULT_CAPACITY);
    if(str->data == NULL) {
        str->capacity = 0;
        return -1;
    }
    else
        str->capacity = OU_STRING_DEFAULT_CAPACITY;

    return 0;
}

int oa_string_init_by_cstr(oa_string_t* str, const char* cstr)
{
    str->len = strlen(cstr);
    str->data = (char*)malloc(sizeof(char)*(str->len + 1));
    if(str->data == NULL)
        return -1;
    strcpy(str->data, cstr);
    str->capacity = str->len + 1;

    return 0;
}

int oa_string_destroy(oa_string_t* str)
{
    if(str->data != NULL) {
        free(str->data);
    }
    str->len = 0;
    str->capacity = 0;

    return 0;
}

int oa_string_assign(oa_string_t* dst, const oa_string_t* src)
{
    char* new_data;    
    unsigned int required_size = src->len + 1;
    if(dst->capacity < required_size)    
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    strcpy(dst->data, src->data);
    dst->len = src->len;

    return 0;
}

int oa_string_assign_cstr(oa_string_t* dst, const char* src)
{
    char* new_data;    
    unsigned int src_length = strlen(src);
    unsigned int required_size = src_length + 1;
    if(dst->capacity < required_size)    
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    strcpy(dst->data, src);
    dst->len = src_length;

    return 0;
}

int oa_string_assign_n(oa_string_t* dst, const char* src, unsigned int count)
{
    char* new_data;    
    unsigned int required_size = count + 1;
    if(dst->capacity < required_size)    
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    strncpy(dst->data, src, count);
    dst->len = count;
    dst->data[dst->len] = '\0';

    return 0;
}

int oa_string_append(oa_string_t* dst, const oa_string_t* src)
{
    char* new_data;
    unsigned int required_size = dst->len + src->len + 1;

    if(dst->capacity < required_size)
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    strcpy(dst->data + dst->len, src->data);
    dst->len += src->len;

    return 0;
}

int oa_string_append_cstr(oa_string_t* dst, const char* src)
{
    char* new_data;
    unsigned int src_length = strlen(src);
    unsigned int required_size = dst->len + src_length + 1;

    if(dst->capacity < required_size)
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    strcpy(dst->data + dst->len, src);
    dst->len += src_length;

    return 0;
}

int oa_string_append_n(oa_string_t* dst, const char* src, unsigned int count)
{
    char* new_data;
    unsigned int required_size = dst->len + count + 1;

    if(dst->capacity < required_size)
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    strncpy(dst->data + dst->len, src, count);
    dst->len += count;
    dst->data[dst->len] = '\0';

    return 0;
}

int oa_string_append_char(oa_string_t* dst, char ch)
{
    char* new_data;
    unsigned int required_size = dst->len + 1 + 1;

    if(dst->capacity < dst->len + 1 + 1) 
    {
        required_size = (unsigned int)
                        (required_size * OU_STRING_EXPAND_FACTOR);
        new_data = (char*)realloc(dst->data, required_size);
        if(new_data == NULL)
            return -1;
        dst->data = new_data;
        dst->capacity = required_size;
    }
    dst->data[dst->len] = ch;
    dst->len ++;
    dst->data[dst->len] = '\0';

    return 0;
}

int oa_string_find(oa_string_t* str, const oa_string_t* partten, char** index)
{
    *index = strstr(str->data, partten->data);
    return 0;
}

int oa_string_find_cstr(oa_string_t* str, const char* partten, char** index)
{
    *index = strstr(str->data, partten);
    return 0;
}

int oa_string_expand(oa_string_t* str, unsigned int new_capacity)
{    
    char* new_data;
    if(str->capacity >= new_capacity)
        return -1;

    new_data = (char*)realloc(str->data, new_capacity);
    if(new_data == NULL)
        return -1;
    str->data = new_data;
    str->capacity = new_capacity;

    return 0;
}

int oa_string_clear(oa_string_t* str) 
{
    str->len = 0; 
    return 0;
}
