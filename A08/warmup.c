Name: Jonathan Hill
Date: Mar. 22, 2023
Description: Warmup.c spawns algorithm processes based on
the image given to us in the assignment description.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
   pid_t pid;
   int child_status;

   printf("%d] A0\n", getpid());
   pid = fork();

   if (pid == 0){
        printf("%d] B1\n", getpid());
        pid = wait(&child_status);
        sleep(5);
     }
   else{
        printf("%d] B0\n", getpid());
        pid = wait(&child_status);
        printf("%d] C0\n" , getpid());
        pid = fork();
   }
    if (pid == 0){
	printf("%d] C1\n", getpid());
    }
  printf("%d] Bye\n", getpid());
  return 0;
}
