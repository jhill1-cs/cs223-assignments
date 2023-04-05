/*
Name: Jonathan Hill
Date: April 5, 2023
Description: search. c uses N threads to search for a value in a list.
The program loads the binary file data.bin and then splits the workload
between the N threads, with N being the user inputted number.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static unsigned long long found = 0;
pthread_mutex_t mutex;

struct thread_information{
  int *array;
  int start;
  int end;
  int target_num;
  int myid;
};

void *thread_search(void* data){

struct thread_information *search_data = (struct thread_information*) data;

for (int i = search_data->start; i < search_data->end; i++){
  if (search_data->array[i] == search_data->target_num){
    pthread_mutex_unlock(&mutex);
    found = 1;
    printf("Thread %d found %d at index %d\n", search_data->myid, search_data->target_num, i);
    pthread_mutex_unlock(&mutex);
    break;
  }
}
return NULL;
}

int main(int argc, char** argv) {
  if (argc < 2){
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }
FILE *file;
file = fopen("data.bin", "rb");
if (!file){
  printf("Incorrect file.");
  exit(0);
}

int size;
fread(&size, sizeof(int), 1, file);

int num_threads = atoi(argv[1]);
pthread_t *thread_number = malloc(num_threads * sizeof(pthread_t));

int thread_size = (int)(size / num_threads);

int *array;
array = (int *)malloc(size * sizeof(int));
fread(array, sizeof(int), size, file);

printf("Enter a value to search: \n");
int user_value;
scanf("%d", &user_value);

struct thread_information *threads = malloc(num_threads * sizeof(struct thread_information));


pthread_mutex_init(&mutex, NULL);
for (int i = 0; i < num_threads; i++){
  threads[i].myid = i;
  threads[i].start = i * thread_size;
  threads[i].end = ((i + 1) * thread_size) -1;
  threads[i].target_num = user_value;
  threads[i].array = array;

  if (i == num_threads-1){
    threads[i].end = size - 1;
  }
  pthread_create(&thread_number[i], NULL, thread_search, (void *) &threads[i]);
}

for (int i = 0; i < num_threads; i++){
  pthread_join(thread_number[i], NULL);
}
if (found == 0){
  printf("Not found!\n");
}

pthread_mutex_destroy(&mutex);
free(array);

  return 0;
}
