#include "oa_array.h"
#include "oa_error_number.h"
#include <string.h>
#include <stdlib.h>

#define OA_ARRAY_DEFAULT_CAPACITY               8
#define OA_ARRAY_EXPAND_FACTORY                 1.5

int32_t oa_array_init(oa_array_t* array, size_t data_size) {
    if(array == NULL || data_size == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    array->data = malloc(data_size * OA_ARRAY_DEFAULT_CAPACITY);
    if(array->data == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    array->data_size = data_size;
    array->size = 0;
    array->capacity = OA_ARRAY_DEFAULT_CAPACITY;

    return OA_ERR_NO_ERROR;
}

int32_t oa_array_destroy(oa_array_t* array) {
    if(array == NULL ) {
        return OA_ERR_ILLEGAL_ARG;
    }
    free(array->data);
    array->data = NULL;
    array->data_size = 0;
    array->size = 0;
    array->capacity = 0;

    return OA_ERR_NO_ERROR;
}

int32_t oa_array_push_back(oa_array_t* array, void* data) {
    size_t new_capacity;
    void* new_data;
    void* next_pos;

    if(array == NULL || data == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if(array->size == array->capacity) {
        new_capacity = (size_t)(array->capacity * OA_ARRAY_EXPAND_FACTORY);
        new_data = realloc(array->data, new_capacity * array->data_size);
        if(new_data == NULL) {
            return OA_ERR_OUT_OF_MEMORY;
        }
        array->data = new_data;
        array->capacity = new_capacity;
    }
    next_pos = (uint8_t*)(array->data) + (array->data_size * array->size);
    switch(array->data_size) {
    case 1: *(uint8_t*) next_pos = *(uint8_t*) data; break;
    case 2: *(uint16_t*) next_pos = *(uint16_t*) data; break;
    case 4: *(uint32_t*) next_pos = *(uint32_t*) data; break;
    case 8: *(uint64_t*) next_pos = *(uint64_t*) data; break;
    default: memcpy(next_pos, data, sizeof(array->data_size)); break;
    }
    array->size ++;

    return OA_ERR_NO_ERROR;
}

int32_t oa_array_pop_back(oa_array_t* array) {
    if(array == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if(array->size == 0) {
        return OA_ERR_OUT_OF_RANGE;
    }
    array->size --;

    return OA_ERR_NO_ERROR;
}

int32_t oa_array_get_cp(oa_array_t* array, size_t index, void* data) {
    void* data_pos;

    if(array == NULL || data == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if(index >= array->size) {
        return OA_ERR_OUT_OF_RANGE;
    }
    data_pos = (uint8_t*)(array->data) + (array->data_size) * index;
    switch (array->data_size) {
    case 1: *(uint8_t*)data = *(uint8_t*)(data_pos); break;
    case 2: *(uint16_t*)data = *(uint16_t*)(data_pos); break;
    case 4: *(uint32_t*)data = *(uint32_t*)(data_pos); break;
    case 5: *(uint64_t*)data = *(uint64_t*)(data_pos); break;
    default: memcpy(data, data_pos, array->data_size); break;        
    }

    return OA_ERR_NO_ERROR;
}

void* oa_array_at(oa_array_t* array, size_t index) {
    return (uint8_t*)(array->data) + (array->data_size) * index;
}

int32_t oa_array_clear(oa_array_t* array) {
    if(array == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    array->size = 0;

    return OA_ERR_NO_ERROR;
}

