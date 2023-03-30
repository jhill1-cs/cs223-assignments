/*
Name: Jonathan Hill
Date: March 29, 2023
Description: warmup.c is a program that uses two
processes to search for a value in a list.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  FILE *file;
  file = fopen("data.bin", "rb");
  int size;
  int child_status;
  int found = 1;

  if (!file){
    printf("Incorrect file.");
    exit(0);
  }

  fread(&size, sizeof(int), 1, file);

  int *array;
  array = (int *)malloc(size * sizeof(int));
  fread(array, sizeof(int), size, file);

  printf("Enter a value to search: ");
  int user_value;
  scanf("%d", &user_value);

  pid_t pid;
  pid = fork();
  int second_half = size / 2;

  // child process
  if (pid == 0){
    for (int i = 0; i < second_half; i++){
      if (array[i] == user_value){
        printf("Child found %d at index %d\n", user_value, i);
        found = 0;
      }
    }
    if (found == 1){
      printf("Not found!\n");
    }
  }
  // parent process
  else{
    pid = wait(&child_status);
    if (found == 1){
      for (int i = second_half; i < size; i++){
        if (array[i] == user_value){
          printf("Parent found %d at index %d\n", user_value, i);
          found = 0;
        }
      }
      if (found == 1){
        printf("Not found!\n");
      }
    }
    else{
      printf("Not found!\n");
    }
  }
  free(array);
  fclose(file);
  return 0;
}

