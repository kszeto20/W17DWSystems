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
    if (semd == 0) {
      printf("Semaphore existed already. No need for a new one\n");
    }
  } else {
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
    printf("Shared memory existed already.\n");
  }
  else {
    printf("Created Shared Memory\n");
  }

  // link to descriptor
  int *data = shmat(shmd, 0, 0);
  //printf("got here1\n");
  //printf("got here2\n");
  // assign to beginning
  (*data) = 0;
  //printf("created shared memory\n");
  //printf("got here3\n");

  close(fd);
}

void remo() {
  // semaphore part
  int semd = semget(SEM_KEY, 0, 0);
  if (semd < 0) {
    semd = semget(SEM_KEY, 1, 0);
    if (semd == 0) {
      printf("Got to Semaphore\n");
    }
  } else  {
    printf("Got to Semaphore\n");
  }

  // removal
  int s = semctl(semd, IPC_RMID, 0);
  if (s < 0) {
    printf("Error removing semaphore %s\n", strerror(errno));
  } else {
    printf("Removed semaphore\n");
  }

  // shared memory part
  int shmd = shmget(SHM_KEY, 0,0);
  // if exists, do this
  if (shmd < 0) {
    shmd = shmget(SHM_KEY, 1, 0);
    if (shmd == 0) {
      printf("Got to Shared Memory\n");
    }
  } else {
    printf("Got to Shared Memory\n");
  }

  // removal
  int r = shmctl(shmd, IPC_RMID, 0);
  if (r < 0) {
    printf("Error removing shared memory %s\n", strerror(errno));
  } else {
    printf("Removed shared memory\n");
  }

  // file part
  int fd = open("story.txt", O_RDONLY);
  if (fd < 0) {
    printf("File does not exist\n");
    return;
  } else  {
    printf("Opened file\n");
  }

  // telphone part
  struct stat sb;
  stat("story.txt", &sb);
  //printf("Got here1\n");
  int size = sb.st_size;
  //printf("Got here0\n");
  char story[size + 1];

  read(fd, story, size);
  story[size] = '\0';

  printf("Storytime:\n %s\n", story);
  close(fd);
}

int main(int argc, char *argv[]) {
  printf("Usage: Add 'create' to create or 'remove' to remove\n");
  if (argv[1] == NULL) {
    printf("Please tell me if you would like to create or remove\n");
    return 0;
  }
  else if (strcmp(argv[1], "create") == 0) {
    create();
  }
  else if (strcmp(argv[1], "remove") == 0) {
    remo();
  }
  return 0;
}
