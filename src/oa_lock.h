#ifndef _OPENAPI_LOCK_H_
#define _OPENAPI_LOCK_H_

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus


typedef void* oa_lock_t;

int oa_lock_init(oa_lock_t* lock);
int oa_lock_destroy(oa_lock_t* lock);
int oa_lock_lock(oa_lock_t* lock);
int oa_lock_try_lock(oa_lock_t* lock, int* is_locked);
int oa_lock_unlock(oa_lock_t* lock);


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_LOCK_H_

