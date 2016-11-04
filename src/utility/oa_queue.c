#include "oa_queue.h"
#include "oa_error_number.h"
#include <stdlib.h>
#include <string.h>

#define OA_QUEUE_DEFAULT_CAPACITY           8
#define OA_QUEUE_EXPAND_FACTOR              1.5


int32_t oa_queue_init(oa_queue_t* queue, size_t data_size) {
    if (queue == NULL || data_size == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    queue->data = malloc(data_size * OA_QUEUE_DEFAULT_CAPACITY);
    if (queue->data == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    queue->capacity = OA_QUEUE_DEFAULT_CAPACITY;
    queue->size = 0;
    queue->data_size = data_size;
    queue->front = 0;
    queue->back = 0;

    return 0;
}

int32_t oa_queue_destroy(oa_queue_t* queue) {
    if (queue == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    free(queue->data);
    queue->data = NULL;

    return 0;
}

int32_t oa_queue_push_back(oa_queue_t* queue, const void* data) {
    size_t new_capacity;
    size_t new_front;
    size_t data_size;   // 單個元素的大小
    void* next_pos;     // 新元素應該加入的位置
    void* new_data;

    if (queue == NULL || data == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    data_size = queue->data_size;
    if (queue->size == queue->capacity) {
        new_capacity = (size_t)(queue->capacity * OA_QUEUE_EXPAND_FACTOR);
        new_data = malloc(new_capacity * data_size);
        if (new_data == NULL) {
            return OA_ERR_OUT_OF_MEMORY;
        }
        memcpy(new_data, queue->data, 
                (queue->back + 1) * data_size);
        new_front = new_capacity - (queue->capacity - queue->front);
        memcpy(new_front, queue->front, 
               (queue->capacity - queue->front) * OA_QUEUE_EXPAND_FACTOR);
        free(queue->data);
        queue->data = new_data;
        queue->capacity = new_capacity;
        queue->front = new_front;
    }
    next_pos = ((uint8_t*)(queue->data)) + data_size * queue->back;
    switch (data_size) {
    case 1: *(uint8_t*) next_pos = *(uint8_t*) data; break;
    case 2: *(uint16_t*) next_pos = *(uint16_t*) data; break;
    case 4: *(uint32_t*) next_pos = *(uint32_t*) data; break;
    case 8: *(uint64_t*) next_pos = *(uint64_t*) data; break;
    default: memcpy(next_pos, data, data_size);  break;
    }
    queue->size ++;
    queue->back = (queue->back + 1) % queue->capacity;

    return 0;
}

int32_t oa_queue_pop_front(oa_queue_t* queue) {
    if (queue == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if (queue->size == 0) {
        return OA_ERR_OUT_OF_RANGE;
    }
    queue->size --;
    queue->front = (queue->front + 1) % queue->capacity;
}

int32_t oa_queue_get_back(oa_queue_t* queue, void** data) {
    if(queue == NULL || data == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if (queue->size == 0) {
        return OA_ERR_OUT_OF_RANGE;
    }
    *data = ((uint8_t*)(queue->data)) + queue->data_size * queue->back;

    return 0;
}

int32_t oa_queue_get_front(oa_queue_t* queue, void** data) {
    if (queue == NULL || data == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if (queue->size == 0) {
        return OA_ERR_OUT_OF_RANGE;
    }
    *data = ((uint8_t*)(queue->data)) + queue->data_size * queue->front;

    return 0;
}

int32_t oa_queue_get_size(oa_queue_t* queue, size_t* size) {
    if(queue == NULL || size == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    *size = queue->size;
    return 0;
}
