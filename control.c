#include "control.h"

void create() {
  int semID = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semID == -1) {
    semID = semget(SEM_KEY, 1, 0);
  }

  union semun us;
  us.val = 10;
  int r = semctl(semID, 0, GETVAL, us);

  printf("SEMCTL RETURNED: %d\n", r);
}

int main() {
  create();
  return 0;
}
