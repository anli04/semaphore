#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define KEY 1432476

int main(int argc, char * argv[]){
  int buf = char[64];
  fgets(buf, sizeof(buf), stdin));
  int shm = shmget(KEY, 10240, IPC_CREAT | IPC_EXCL);
  int sem = semget(KEY, 1, IPC_CREAT | IPC_EXCL);
  int fd = open("text.txt", O_CREAT | O_TRUNC, 0644);
  return 0;
}
