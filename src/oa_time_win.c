#include "oa_time.h"
#include "oa_error_number.h"
#include <Windows.h>

//int oa_time_get_local_time_s(unsigned long long* time);
//int oa_time_get_local_time_ms(unsigned long long* time);

int oa_time_get_tick_count_64(unsigned long long* tick_count) {
    if (tick_count == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    *tick_count = GetTickCount64();
    return 0;
}

int oa_time_sleep_s(unsigned int second) {
    if (second > 0xffffffff / 1000)
        return OA_ERR_OUT_OF_RANGE;
    Sleep(second * 1000);
    return 0;
}

int oa_time_sleep_ms(unsigned int millisecond) {
    Sleep(millisecond);
    return 0;
}

