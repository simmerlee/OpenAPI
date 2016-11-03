#include "oa_thread.h"
#include <stdio.h>
#include <unistd.h>

void run(void* arg){
    printf("Hello World from sub thread.\n");
    printf("arg is: %s\n", (char*)arg);
    sleep(16);
    printf("Goodbye.\n");
}

int main(){
    oa_thread_t thread;
    int ret;
    unsigned int tid;
    char buf[64];
    pid_t pid;

    ret = oa_thread_init(&thread, run, NULL);
    if(ret != 0)
        printf("init failed. ret:%d\n", ret);
    ret = oa_thread_start(&thread, "hahaha~");
    if(ret != 0)
        printf("start failed. ret:%d\n", ret);
        
    // wait to assure sub-thread is started.
    sleep(2);

    ret = oa_thread_get_tid(&thread, &tid);
    if(ret != 0)
        printf("gettid failed. ret:%d\n", ret);
    printf("tid:%u\n", tid);
    ret = oa_thread_get_name(&thread, buf, 64);
    if(ret != 0)
        printf("get name failed. ret:%d\n", ret);
    printf("thread name: %s\n", buf);
    ret = oa_thread_join(&thread);
    if(ret != 0)
        printf("join failed. ret:%d\n", ret);
    ret = oa_thread_destroy(&thread);
    if(ret != 0)
        printf("destroy failed. ret:%d\n", ret);

    return 0;
}


