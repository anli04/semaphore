ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control write

control: control.c
	$(CC) -o control control.c

write: write.c
	$(CC) -o write write.c

memcheck:
	valgrind --leak-check=yes ./test

clean:
	rm *.o
	rm *~
