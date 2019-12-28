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
#define SIZE 1024

void errcheck(char * m);

int main(){
  int shm;
  shm = shmget(KEY, SIZE, 0);
  errcheck("accessing shared memory");
  char * mem;
  mem = shmat(shm, 0, 0);
  errcheck("attaching shared memory");
  int sem;
  sem = semget(KEY, 1, 0);
  errcheck("getting semaphore");
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  printf("getting in...\n");
  semop(sem, &sb, 1);
  errcheck("getting semaphore");
  printf("Last addition:\n%s\n", mem);
  printf("Your addition:\n");
  char text[SIZE];
  fgets(mem, SIZE, stdin);
  *strchr(mem, '\n') = 0;
  strcpy(text, mem);
  int fd = open("test.txt", O_WRONLY);
  errcheck("opening file");
  write(fd, text, strlen(text));
  write(fd, "\n", 1);
  errcheck("writing to file");
  sb.sem_op = 1;
  semop(sem, &sb, 1);
  errcheck("updating semaphore");
  shmdt(mem);
  errcheck("detaching shared memory");
  return 0;
}

void errcheck(char * m){
  if (errno){
    printf("Error %s: %d - %s\n", m, errno, strerror(errno));
    errno = 0;
  }
}
