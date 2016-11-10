#ifndef _OPENAPI_ARRAY_H_
#define _OPENAPI_ARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>
#include <stddef.h>

typedef struct {
    void* data;
    size_t data_size;
    size_t size;
    size_t capacity;
} oa_array_t;

int32_t oa_array_init(oa_array_t* array, size_t data_size);
int32_t oa_array_destroy(oa_array_t* array);
int32_t oa_array_push_back(oa_array_t* array, void* data);
int32_t oa_array_pop_back(oa_array_t* array);
int32_t oa_array_get_cp(oa_array_t* array, size_t index, void* data);
// 把index位置的元素內容拷貝到data中

void* oa_array_at(oa_array_t* array, size_t index); // don't check range
// 返回index位置元素的指針

int32_t oa_array_clear(oa_array_t* array);


#ifdef __cplusplus
}
#endif//__cplusplus


#endif//_OPENAPI_ARRAY_H_

