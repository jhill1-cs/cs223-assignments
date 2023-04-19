/*
Name: Jonathan Hill
Date: April 19, 2023
Description: memstats.c prints the total number of
memory blocks, total amount of memory, and 
underutilized memory as a percentage.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  int blocks_used = 0;
  int memory_use = 0;
  int blocks_freed = 0;
  int memory_free = 0;
  int total_used = 0;
  float underutilized_percentage;

  for (int i = 0; i < len; i++){
    if (buffer[i] != NULL){
      blocks_used += 1;
      struct chunk* cnk = (struct chunk*)buffer[i]-1;
      memory_use += cnk->size;
      total_used += cnk->used;
    }
  }
  struct chunk *next = freelist;
  while(next!= NULL){
    blocks_freed += 1; 
    memory_free += next->size;
    next = next->next;
    
  }
  underutilized_percentage = (float) (memory_use - total_used) / (memory_use);
  printf("Total blocks: %d Free blocks: %d Used blocks: %d\n", 
  (blocks_used + blocks_freed) , blocks_freed , blocks_used);
  printf("Total memory allocated: %d Free memory: %d Used memory: %d\n", 
  (memory_free + memory_use), memory_free, memory_use);
  printf("Underutilized memory: %f\n", underutilized_percentage);

}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
