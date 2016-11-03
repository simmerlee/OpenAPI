#include "oa_thread.h"
#include "oa_lock.h"
#include <stdio.h>
#include <unistd.h>

void run1(void* arg) {
    oa_lock_t* lock = (oa_lock_t*)arg;
    unsigned int count;

    printf("thread 1 lock first.\n");
    if(oa_lock_lock(lock) != 0) {
        printf("thread 1 lock failed\n");
        return ;
    }
    sleep(2);
    printf("thread 1 unlock\n");
    if(oa_lock_unlock(lock) != 0){
        printf("thread 2 unlock failed\n");
        return ;
    }
    sleep(2);

    for(count = 0; count < 16; count++) {
        if(oa_lock_lock(lock) != 0) {
            printf("thread 1 lock failed.\n");
            return;
        }
        printf("thread 1 \n");
        if(oa_lock_unlock(lock) != 0) {
            printf("thread 1 unlock failed.\n");
        }
        usleep(500*1000);
    }
}

void run2(void* arg) {
    oa_lock_t* lock = (oa_lock_t*)arg;
    unsigned int count;
    int is_lock;

    sleep(1);
    if(oa_lock_try_lock(lock, &is_lock) != 0) {
        printf("thread 2 try lock failed.\n");
        return ;
    }
    printf("thread 2 try lock, is_locked: %d\n", is_lock);
    sleep(2);
    if(oa_lock_try_lock(lock, &is_lock) != 0) {
        printf("thread 2 try lock failed.\n");
        return ;
    }
    printf("thread 2 try lock, is_locked: %d\n", is_lock);
    sleep(1);
    if(oa_lock_unlock(lock) != 0) {
        printf("thread 2 unlock failed.\n");
        return ;
    }
    printf("thread 2 unlock\n");

    for(count = 0; count < 16; count++) {
        if(oa_lock_lock(lock) != 0) {
            printf("thread 2 lock failed.\n");
            return;
        }
        printf("thread 2 \n");
        if(oa_lock_unlock(lock) != 0) {
            printf("thread 2 unlock failed.\n");
        }
        usleep(500*1000);
    }
}

int main() {
    oa_thread_t t1, t2;
    oa_lock_t lock;

    if(oa_thread_init(&t1, run1, NULL) != 0) {
        printf("thread init failed 1.\n");
        return 0;
    }
    if(oa_thread_init(&t2, run2, NULL) != 0) {
        printf("thread init failed 2.\n");
        return 0;
    }
    if(oa_lock_init(&lock) != 0) {
        printf("lock init failed.\n");
        return 0;
    }
    if(oa_thread_start(&t1, &lock) != 0) {
        printf("thread start failed. 1\n");
        return 0;
    }
    if(oa_thread_start(&t2, &lock) != 0) {
        printf("thread start failed. 2\n");
        return 0;
    }
    if(oa_thread_join(&t1) != 0) {
        printf("thread join failed. 1\n");
        return 0;
    }
    if(oa_thread_join(&t2) != 0) {
        printf("thread join failed. 2\n");
        return 0;
    }
    if(oa_thread_destroy(&t1) != 0) {
        printf("thread destroy failed. 1\n");
        return 0;
    }
    if(oa_thread_destroy(&t2) != 0) {
        printf("thread destroy failed. 2\n");
        return 0;
    }
    if(oa_lock_destroy(&lock) != 0) {
        printf("lock destroy failed.\n");
        return 0;
    }

    return 0;
}
