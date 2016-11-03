#include "oa_lock.h"
#include "oa_error_number.h"
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

typedef struct {
    pthread_mutex_t handle;
}oa_lock_private_t;


int oa_lock_init(oa_lock_t* lock) {
    oa_lock_private_t* lp;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)malloc(sizeof(oa_lock_private_t));
    if(lp == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    if(pthread_mutex_init(&(lp->handle), NULL) != 0) {
        free(lp);
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    *lock = lp;

    return 0;
}

int oa_lock_destroy(oa_lock_t* lock) {
    oa_lock_private_t* lp;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)(*lock);
    if(pthread_mutex_destroy(&(lp->handle)) != 0) {
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    free(lp);

    return 0;
}

int oa_lock_lock(oa_lock_t* lock) {
    oa_lock_private_t* lp;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)(*lock);
    if(lp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    if(pthread_mutex_lock(&(lp->handle)) != 0) {
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return 0;
}

int oa_lock_try_lock(oa_lock_t* lock, int* is_locked) {
    oa_lock_private_t* lp;
    int ret;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)(*lock);
    ret = pthread_mutex_trylock(&(lp->handle));
    if(lp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    if(ret == 0) {
        *is_locked = 1;
        return 0;
    }
    if(ret == EBUSY) {
        is_locked = 0;
        return 0;
    }
    return OA_ERR_SYSTEM_CALL_FAILED;
}

int oa_lock_unlock(oa_lock_t* lock) {
    oa_lock_private_t* lp;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)(*lock);
    if(lp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    if(pthread_mutex_unlock(&(lp->handle)) != 0) {
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return 0;
}

