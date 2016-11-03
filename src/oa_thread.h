#ifndef _OPENAPI_THREAD_H_
#define _OPENAPI_THREAD_H_

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus


typedef void* oa_thread_t;

int oa_thread_init(oa_thread_t* thread, 
                   void (*thread_run)(void* args),
                   char* thread_name);
int oa_thread_destroy(oa_thread_t* thread);

int oa_thread_start(oa_thread_t* thread, void* args);

int oa_thread_join(oa_thread_t* thread);

int oa_thread_get_tid(oa_thread_t* thread, unsigned int* tid);
// Notice:
// This function will return error result if
// this function called before sub-thread really started

int oa_thread_get_name(oa_thread_t* thread, 
                       char* buf, unsigned int buf_size);

int oa_thread_get_current_thread_id(unsigned int* tid);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_THREAD_H_

