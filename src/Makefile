cc = gcc
cflags = -W -c
target = libOpenAPI.a
objs = oa_thread.o oa_lock.o oa_time.o

$(target) : $(objs)
	ar -cvr $(target) $(objs)
oa_thread.o : oa_thread_linux.c oa_thread.h
	$(cc) $(cflags) $< -o $@
oa_lock.o : oa_lock_linux.c oa_lock.h
	$(cc) $(cflags) $< -o $@
oa_time.o : oa_time_linux.c oa_time.h
	$(cc) $(cflags) $< -o $@

clean :
	rm -f $(objs) $(target)

