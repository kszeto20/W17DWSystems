#include "control.h"

void create() {

  // file open
  int fd = open("story.txt", O_CREAT | O_EXCL | O_TRUNC, 0644);
  if (errno != 0) {
      fd = open("story.txt", O_RDWR);
      printf("Opened File\n");
  } else {
    printf("Opened File\n");
  }

  // semaphore create
  int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644); // will fail if already exits
    // if exists; do this
  if (semd < 0) {
    semd = semget(SEM_KEY, 1, 0);
  }
  if (semd == 0) {
    printf("Created Semaphore\n");
  }

  // semaphore set
  union semun us;
  us.val = 1;
  int r = semctl(semd, 0, SETVAL, us);
  printf("Set Semaphore\n");
  //printf("got here\n");

  //shared memory create
  int shmd = shmget(SHM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  // if exists, do this
  if (shmd < 0) {
    shmd = shmget(SHM_KEY, 0, 0);
  }

  // link to descriptor
  int *data = shmat(shmd, 0, 0);
  //printf("got here1\n");
  //printf("got here2\n");
  // assign to beginning
  (*data) = 0;
  printf("created shared memory\n");
  //printf("got here3\n");

  close(fd);
}

int main() {
  create();
  return 0;
}
