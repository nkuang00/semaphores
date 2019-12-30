#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define KEY_SEM 24602

#define KEY 24601
#define SEG_SIZE 200

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};


int main(int argc, char * argv[]) {

  int shmd;
  int semd;
  int v, r;
  char input[3];
  char req[3];

  if (argc == 1) {
    printf("please enter a command (-c, -r, -v): ");
    fflush(stdout);
    fgets(req, 3, stdin);
  }
  else{
    strcpy(req, argv[1]);

  }
  while (strcmp(req, "-c") != 0 && strcmp(req, "-r") != 0 && strcmp(req, "-v") != 0){
    printf("please enter a valid command (-c, -r, -v): ");
    fflush(stdout);
    fgets(req, 3, stdin);
  }

  if (strcmp(req, "-c") == 0){
    //create stuff

    //shm
    shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    printf("shared memory created\n");

    //semaphore
    semd = semget(KEY_SEM, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
      printf("error %d: %s\n", errno, strerror(errno));
      semd = semget(KEY_SEM, 1, 0);
    }
    else {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
      printf("semaphore created\n");
    }

    //create file
    int f = fork();
    char * args[2];
    int status;
    args[0] = "touch";
    args[1] = "story";
    if (f == 0){
      execvp(args[0], args);

    }
    else{
      wait(&status);
      printf("file created\n\n");
    }

    return 0;
  }

  //check resources
  FILE * f;
  f = fopen("story", "r");

  if (strcmp(req, "-r") == 0){
    //try to access semaphore
    printf("the story so far:\n");
    char str[200];
    while (fgets(str, 200, f)){
      fputs(str, stdout);
    }
    fclose(f);
    printf("\n");


    //remove semaphore
    semd = semget(KEY_SEM, 1, 0);

    semctl(semd, IPC_RMID, 0);
    printf("semaphore deleted\n");

    //remove shared mem
    shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    shmctl(shmd, IPC_RMID, 0);
    printf("shared memory removed\n");

    //remove file
    int f = fork();
    char * args[2];
    int status;
    args[0] = "rm";
    args[1] = "story";
    if (f == 0){
      execvp(args[0], args);
    }
    else{
      wait(&status);

      printf("file removed\n");
    }

  }

  if (strcmp(req, "-v") == 0){
    printf("the story so far:\n");
    char str[200];
    while (fgets(str, 200, f)){
      fputs(str, stdout);
    }
    fclose(f);
    printf("\n");

  }


  return 0;
}
