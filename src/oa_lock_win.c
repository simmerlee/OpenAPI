#include "oa_lock.h"
#include "oa_error_number.h"
#include <stdlib.h>
#include <errno.h>
#include <Windows.h>

typedef struct {
    CRITICAL_SECTION handle;
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
    InitializeCriticalSection(&(lp->handle));
    *lock = lp;

    return 0;
}

int oa_lock_destroy(oa_lock_t* lock) {
    oa_lock_private_t* lp;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)(*lock);
    DeleteCriticalSection(&(lp->handle));
    free(lp);
    *lock = NULL;

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
    EnterCriticalSection(&(lp->handle));

    return 0;
}

int oa_lock_try_lock(oa_lock_t* lock, int* is_locked) {
    oa_lock_private_t* lp;
    BOOL ret;

    if(lock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    lp = (oa_lock_private_t*)(*lock);
    if(lp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }

    // return non-zero: successfully entered
    // return zero: another thread already owns the critical section
    // by MSDN
    ret = TryEnterCriticalSection(&(lp->handle));
    if(ret != 0) {
        *is_locked = 1;
        return 0;
    }
    if(ret == 0) {
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
    LeaveCriticalSection(&(lp->handle));

    return 0;
}

