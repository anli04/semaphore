ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control write

control: control.c
	$(CC) cc -o control control.c

write: write.c
	$(CC) cc -o write write.c

memcheck:
	valgrind --leak-check=yes ./test

clean:
	rm *.o
	rm *~
