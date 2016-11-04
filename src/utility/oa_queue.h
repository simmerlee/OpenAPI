#ifndef _OPENAPI_QUEUE_H_
#define _OPENAPI_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>
#include <stddef.h> // for size_t

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    size_t data_size;
    size_t front;
    size_t back;
} oa_queue_t;

int32_t oa_queue_init(oa_queue_t* queue, size_t data_size);
int32_t oa_queue_destroy(oa_queue_t* queue);
int32_t oa_queue_push_back(oa_queue_t* queue, const void* data);
int32_t oa_queue_pop_front(oa_queue_t* queue);
int32_t oa_queue_get_back(oa_queue_t* queue, void** data);
int32_t oa_queue_get_front(oa_queue_t* queue, void** data);
int32_t oa_queue_get_size(oa_queue_t* queue, size_t* size);


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_QUEUE_H_
