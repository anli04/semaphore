#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>

#define KEY 1432476

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
  //char * mem;
  if (!strcmp(argv[1], "-c")){
    shm = shmget(KEY, 1024, IPC_CREAT | 0644);
    errcheck("creating shared memory");
    /*mem = shmat(shm, NULL, 0);
    errcheck("attaching shared memory");*/
    sem = semget(KEY, 1, IPC_CREAT | 0644);
    errcheck("creating semaphore");
    union semun us;
    us.val = 1;
    semctl(sem, 0, SETVAL, us);
    errcheck("setting semaphore");
    fd = open("text.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    errcheck("creating file");
    close(fd);
  }
  else if(!strcmp(argv[1], "-r")){
    fd = open("text.txt", O_RDONLY | O_TRUNC);
    errcheck("opening file");

    shm = shmget(KEY, 1024, 0);
    errcheck("getting shared memory");
    shmctl(shm, IPC_RMID, 0);
    errcheck("removing shared memory");
    sem = semget(KEY, 1, 0);
    errcheck("getting semaphore");
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(sem, &sb, 1);
    errcheck("getting semaphore");
    shmctl(sem, IPC_RMID, 0);
    errcheck("removing semaphore");
    )
  }
  else if(!strcmp(argv[1], "-v")){
    fd = open("text.txt", O_RDONLY | O_TRUNC);
    errcheck("opening file");
    char text[1024];
    read(fd, text, 1024);
    printf("%s\n", text);
    closed(fd);
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
