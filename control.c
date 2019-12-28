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
  if(argc < 2){
    printf("Error: missing flag\n");
    return 0;
  }
  if (!strcmp(argv[1], "-c")){
    shm = shmget(KEY, SIZE, IPC_CREAT | 0644);
    errcheck("creating shared memory");
    sem = semget(KEY, 1, IPC_CREAT | 0644);
    errcheck("creating semaphore");
    union semun s;
    s.val = 1;
    semctl(sem, 0, SETVAL, s);
    errcheck("setting semaphore");
    fd = open("text.txt", O_CREAT | O_EXCL, 0644);
    if (errno == EEXIST) errno = 0;
    errcheck("creating file");
    close(fd);
  }
  else if(!strcmp(argv[1], "-r")){
    sem = semget(KEY, 1, 0);
    errcheck("getting semaphore");
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    printf("waiting...\n");
    semop(sem, &sb, 1);
    errcheck("getting semaphore");
    FILE* f = fopen("text.txt", "r");
    errcheck("opening file");
    printf("The Story:\n");
    char line[SIZE];
    while (fgets(line, sizeof(line), f)) {
      *strchr(line, '\n') = 0;
      printf("%s\n", line);
    }
    printf("\n");
    fclose(f);
    close(open("text.txt", O_WRONLY | O_TRUNC));
    shm = shmget(KEY, SIZE, 0);
    errcheck("getting shared memory");
    shmctl(shm, IPC_RMID, 0);
    errcheck("removing shared memory");
    semctl(sem, IPC_RMID, 0);
    errcheck("removing semaphore");
    printf("Removal complete\n");
  }
  else if(!strcmp(argv[1], "-v")){
    FILE* f = fopen("text.txt", "r");
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
