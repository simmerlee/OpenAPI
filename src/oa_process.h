#ifndef _OPENAPI_PROCESS_H_
#define _OPENAPI_PROCESS_H_

typedef void* oa_process_t;

int oa_process_create();
int oa_process_wait();
int oa_process_kill();
int oa_process_get_current_process_id();

#endif//_OPENAPI_PROCESS_H_

