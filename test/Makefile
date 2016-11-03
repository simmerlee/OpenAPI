cc = gcc
cflags = -W -I../src
target = thread_test thread_lock_test time_test
lib = ../src/libOpenAPI.a
link = -lpthread

all : $(target)	
thread_test : thread_test.c $(lib)
	$(cc) $(cflags) $< $(lib) -o $@ $(link)
thread_lock_test : thread_lock_test.c $(lib)
	$(cc) $(cflags) $< $(lib) -o $@ $(link)
time_test : time_test.c $(lib)
	$(cc) $(cflags) $< $(lib) -o $@ $(link)

clean :
	rm -f $(target)

