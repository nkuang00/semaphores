#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY 24601
#define SEG_SIZE 200

int main() {

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
  fgets(input, 0, stdin);

  fgets(data, SEG_SIZE, stdin);
  *strchr(data, '\n') = 0;
  printf("Current contents: [%s]\n", data);


  shmdt(data);

  // printf("Would you like to remove the segment?(y/n) ");
  // fgets(input, 3, stdin);

  // if (input[0] == 'y') {
  //   shmctl(shmd, IPC_RMID, 0);
  //   printf("segment deleted\n");
  // }

  return 0;
}
