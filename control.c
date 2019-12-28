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

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

void errcheck(char * m);

int main(int argc, char * argv[]){
  int shm;
  int sem;
  int fd;
  union semun su;
  su.val = 1;
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  if(argc < 2){
    printf("Error: missing flag\n");
    return 0;
  }
  if (!strcmp(argv[1], "-c")){
    shm = shmget(KEY, SIZE, IPC_CREAT | 0644);
    errcheck("creating shared memory");
    printf("Shared Memory created\n");
    sem = semget(KEY, 1, IPC_CREAT | 0644);
    errcheck("creating semaphore");
    printf("Semaphore created\n");
    semctl(sem, 0, SETVAL, su);
    errcheck("setting semaphore");
    FILE * f = fopen("story", "w");
    errcheck("creating file");
    fclose(f);
    printf("File created\n");
  }
  else if(!strcmp(argv[1], "-r")){
    sem = semget(KEY, 1, 0);
    errcheck("getting semaphore");
    printf("waiting...\n");
    semop(sem, &sb, 1);
    errcheck("getting semaphore");
    FILE* f = fopen("story", "r");
    errcheck("opening file");
    printf("The Story:\n");
    char line[SIZE];
    while (fgets(line, sizeof(line), f)) {
      *strchr(line, '\n') = 0;
      printf("%s\n", line);
    }
    printf("\n");
    fclose(f);
    shm = shmget(KEY, SIZE, 0);
    errcheck("getting shared memory");
    shmctl(shm, IPC_RMID, 0);
    errcheck("removing shared memory");
    semctl(sem, IPC_RMID, 0);
    errcheck("removing semaphore");
    printf("Removal complete\n");
  }
  else if(!strcmp(argv[1], "-v")){
    FILE* f = fopen("story", "r");
    errcheck("opening file");
    printf("The story so far:\n");
    char line[SIZE];
    while (fgets(line, sizeof(line), f)) {
      *strchr(line, '\n') = 0;
      printf("%s\n", line);
    }
    fclose(f);
  }
  else {
    printf("ERROR\n");
  }
  return 0;
}

void errcheck(char * m){
  if (errno){
    printf("Error %s: %d - %s\n", m, errno, strerror(errno));
    errno = 0;
  }
}
