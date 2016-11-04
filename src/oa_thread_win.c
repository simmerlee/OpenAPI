#include "oa_thread.h"
#include "oa_error_number.h"
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#define OA_THREAD_NAME_MAX_LENGTH           63
#define OA_THREAD_DEFAULT_NAME              "oa_thread"

typedef struct{
    HANDLE       tp_handle;
    unsigned int    tp_thread_id;
    void*           tp_args;
    char            tp_thread_name[OA_THREAD_NAME_MAX_LENGTH];
    void            (*tp_run)(void* args);
}oa_thread_private_t;

typedef struct 
{
    DWORD dwType; // must be 0x1000
    LPCSTR szName; // pointer to name (in user addr space)
    DWORD dwThreadID; // thread ID (-1=caller thread)
    DWORD dwFlags; // reserved for future use, must be zero
} THREADNAME_INFO;

void SetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
{
#ifdef WIN32
#if _WIN64 != 1
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException(0x406D1388, 0,
            sizeof(info) / sizeof(DWORD),
            (DWORD*)&info);
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
#endif//_WIN64 != 1
#endif//WIN32
}

static unsigned int __stdcall oa_thread_run_wrapper(void* thread_private) {
    oa_thread_private_t* tp = thread_private;

    // set thread name
    SetThreadName(GetCurrentThreadId(), tp->tp_thread_name);

    // get new thread id by using system call
    tp->tp_thread_id = GetCurrentThreadId();

    // run thread
    tp->tp_run(tp->tp_args);
    
    return 0;
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
    oa_thread_private_t* tp;
   
    if(thread == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    tp = (oa_thread_private_t*)(*thread);
    if(tp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    
    tp->tp_args = args;
    tp->tp_handle = (HANDLE)_beginthreadex(
                                NULL,   // security
                                0,      // stack size
                                oa_thread_run_wrapper, // routine
                                tp,     // arg
                                0,      // init_flag
                                NULL);  // thread_addr
    if(tp->tp_handle == NULL) {
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
    if (WaitForSingleObject(tp->tp_handle, INFINITE) == WAIT_OBJECT_0) {
        return OA_ERR_NO_ERROR;
    }

    return OA_ERR_SYSTEM_CALL_FAILED;
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
    *tid = GetCurrentThreadId();

    return 0;
}

