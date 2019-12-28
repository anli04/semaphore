#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>

#define KEY 1432476

void errcheck();

int main(){
  int shm;
  shm = shmget(KEY, 1024, 0);
  errcheck("accessing shared memory");
  int sem;
  sem = semget(KEY, 1, 0);
  errcheck("getting semaphore");
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(sem, &sb, 1);
  errcheck("getting semaphore");
  char text[1024];
  fgets(text, 1024, stdin);
  int fd = open("test.txt", O_WRONLY);
  write(fd, text, strlen(text));
  write(fd, "\n", 1);
  sb.sem_op = 1;
  semop(sem, &sb, 1);
  errcheck("updating semaphore");
  return 0;
}

void errcheck(){
  if (errno){
    printf("Error: %d - %s\n", errno, strerror(errno));
    errno = 0;
  }
}
