#include "oa_time.h"
#include <stdio.h>

int main() {
    unsigned long long time;
    oa_time_get_local_time_s(&time);
    printf("Current second since epoch: %llu\n", time);
    oa_time_get_local_time_ms(&time);
    printf("Current millisecond since epoch: %llu\n", time);
    oa_time_get_tick_count_64(&time);
    printf("Millisecond since power on: %llu\n", time);

    printf("Sleep 1 second...\n");
    oa_time_sleep_s(1);
    printf("Sleep 123 millisecond...\n");
    oa_time_sleep_ms(123);

    return 0;
}

