#ifndef _OPENAPI_TIME_H_
#define _OPENAPI_TIME_H_

int oa_time_get_local_time_s(unsigned long long* time);
int oa_time_get_local_time_ms(unsigned long long* time);
int oa_time_get_tick_count_64(unsigned long long* tick_count);

int oa_time_sleep_s(unsigned int second);
int oa_time_sleep_ms(unsigned int millisecond);

#endif//_OPENAPI_TIME_H_

