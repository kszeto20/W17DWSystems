#include "control.h"

void create() {

  // file open
  int fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
  if (errno != 0) {
      printf("Error: %s\n", strerror(errno));
      exit(errno);
  }
  // semaphore create
  int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644); // will fail if already exits
    // if exists; do this
  if (semd == -1) {
    semd = semget(SEM_KEY, 1, 0);
  }

  union semun us;
  us.val = 1;
  int r = semctl(semd, 0, SETVAL, us);
  printf("got here\n");

  //shared memory create
  int shmd = shmget(SHM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  // if exists, do this
  if (shmd == -1) {
    shmd = shmget(SHM_KEY, 0, 0);
  }

  printf("got here1\n");
  // link to descriptor
  int *data = shmat(shmd, 0, 0);

  printf("got here2\n");
  close(fd);
  // assign to beginning
  (*data) = 0;
  printf("got here3\n");
}

int main() {
  create();
  return 0;
}
