#include "oa_thread.h"
#include "oa_error_number.h"
#include <pthread.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <string.h>

#define OA_THREAD_NAME_MAX_LENGTH           63
#define OA_THREAD_DEFAULT_NAME              "oa_thread"

typedef struct{
    pthread_t       tp_handle;
    unsigned int    tp_thread_id;
    void*           tp_args;
    char            tp_thread_name[OA_THREAD_NAME_MAX_LENGTH];
    void            (*tp_run)(void* args);
}oa_thread_private_t;

static void* oa_thread_run_wrapper(void* thread_private) {
    oa_thread_private_t* tp = thread_private;

    // set thread name
    // don't handle error 
    // because Kernel version lower than 2.6.9 dosen't support
	prctl(PR_SET_NAME, tp->tp_thread_name);

    // get new thread id by using system call
    oa_thread_get_current_thread_id(&(tp->tp_thread_id));

    // run thread
    tp->tp_run(tp->tp_args);
    
    return NULL;
}

int oa_thread_init(oa_thread_t* thread,
                   void (*thread_run)(void* args),
                   char* thread_name) {
    oa_thread_private_t* tp;
    if(thread == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    tp = (oa_thread_private_t*)malloc(sizeof(oa_thread_private_t));
    if(tp == NULL){
        return OA_ERR_OUT_OF_MEMORY;
    }
    tp->tp_run = thread_run;

    if(thread_name != NULL) {
        if(strlen(thread_name) > OA_THREAD_NAME_MAX_LENGTH) {
            free(tp);
            return OA_ERR_CONTENT_TOO_LONG;
        }
        strcpy(tp->tp_thread_name, thread_name);
    }
    else {
        strcpy(tp->tp_thread_name, OA_THREAD_DEFAULT_NAME);
    }
    *thread = tp;

    return OA_ERR_NO_ERROR;
}

int oa_thread_destroy(oa_thread_t* thread){
    if(thread == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if(*thread == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    free(*thread);
    (*thread) = NULL;
    
    return OA_ERR_NO_ERROR;
}

int oa_thread_start(oa_thread_t* thread, void* args) {
    int ret;
    oa_thread_private_t* tp;
   
    if(thread == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    tp = (oa_thread_private_t*)(*thread);
    if(tp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    
    tp->tp_args = args;
    ret = pthread_create(&(tp->tp_handle),
                         NULL, 
                         oa_thread_run_wrapper,
                         tp);
    if(ret != 0) {
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return OA_ERR_NO_ERROR;
}

int oa_thread_join(oa_thread_t* thread) {
    oa_thread_private_t* tp;

    if(thread == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    tp = (oa_thread_private_t*)(*thread);
    if(tp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    if(pthread_join(tp->tp_handle, NULL) != 0) {
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return OA_ERR_NO_ERROR;
}

int oa_thread_get_tid(oa_thread_t* thread, unsigned int* tid) {
    oa_thread_private_t* tp;

    if(thread == NULL || tid == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    tp = (oa_thread_private_t*)(*thread);
    if(tp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    *tid = tp->tp_thread_id;

    return OA_ERR_NO_ERROR;
}

int oa_thread_get_name(oa_thread_t* thread, 
                       char* buf, unsigned int buf_size) {
    oa_thread_private_t* tp;

    if(thread == NULL || buf == NULL || buf_size == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    tp = (oa_thread_private_t*)(*thread);
    if(tp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    if(strlen(tp->tp_thread_name) > buf_size) {
        return OA_ERR_BUFFER_TOO_SMALL;
    }
    strcpy(buf, tp->tp_thread_name);

    return OA_ERR_NO_ERROR;
}

int oa_thread_get_current_thread_id(unsigned int* tid) {
    if(tid == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    *tid = (unsigned int)syscall(__NR_gettid);

    return 0;
}

