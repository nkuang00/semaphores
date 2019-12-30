#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>

#define KEY_SEM 24602


#define KEY 24601
#define SEG_SIZE 200

int main() {

  int semd;
  int r;
  int v;
  FILE * f;

  semd = semget(KEY_SEM, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;

  semop(semd, &sb, 1);

  f = fopen("story", "a");

  //sb.sem_flg = SEM_UNDO;


  //--------------------

  int shmd;
  char * data;
  char input[3];

  shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
  data = shmat(shmd, 0, 0);

  printf("Last addition: ");

  if (!(*data))
    printf("\n");
  else
    printf("%s\n", data);

  printf("Your addition: ");
  fgets(data, SEG_SIZE, stdin);
  fputs(data, f);



  shmdt(data);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  fclose(f);
  printf("\n");

  return 0;
}
