#include "oa_time.h"
#include "oa_error_number.h"

#include <unistd.h>
#include <sys/time.h>
#include <time.h>

// TODO:
// Handle sleep() return value
// when sleep() is interupt

int oa_time_get_local_time_s(unsigned long long* time) {
    struct timeval tv;

    if(time == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    gettimeofday(&tv, NULL);
    *time = (unsigned long long)(tv.tv_sec);
    return 0;
}

int oa_time_get_local_time_ms(unsigned long long* time) {
    struct timeval tv;

    if(time == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    gettimeofday(&tv, NULL);
    *time = (unsigned long long)(tv.tv_sec) * 1000ULL
          + (unsigned long long)(tv.tv_usec / 1000);
    return 0;
}

int oa_time_get_tick_count_64(unsigned long long* tick_count) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    *tick_count = ts.tv_sec * 1000ULL 
                + ts.tv_nsec / 1000000ULL;
}

int oa_time_sleep_s(unsigned int second) {
    sleep(second);
    return 0;
}

int oa_time_sleep_ms(unsigned int millisecond) {
    usleep(millisecond * 1000);
}

