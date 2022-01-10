#include "write.h"

int main() {
  // semaphore
  int semd = semget(SEM_KEY, 1, 0);
  if (semd < 0) {
    printf("Error getting semaphore %s\n", strerror(errno));
  } else  {
    printf("Got to Semaphore\n");
  }

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);

  // shared memory
  int shmd = shmget(SHM_KEY, 0, 0);
  if (shmd < 0) {
    printf("Error getting to shared memory %s\n", strerror(errno));
  } else  {
    printf("Got to shared memory\n");
  }

  char *length = shmat(shmd, 0, 0);
  if (length < 0) {
    printf("Error getting length of shared memory %s\n", strerror(errno));
  }

  char words[1024];

  int fd = open("story.txt", O_RDWR | O_APPEND);

  lseek(fd, -(*length), SEEK_END);
  read(fd, words, sizeof(words));
  words[*length] = '\0';

  printf("Previous Line: %s\n", words);

  char newWords[1024];
  printf("Enter a new line to the story: ");
  fgets(newWords, sizeof(newWords), stdin);

  lseek(fd, 0, SEEK_END);
  write(fd, newWords, sizeof(newWords));
  *length = strlen(newWords);
  printf("'%s' has been added to the story\n", newWords);


  shmdt(length);

  sb.sem_op = 1;
  semop(semd, &sb, 1);
  close(fd);
}
