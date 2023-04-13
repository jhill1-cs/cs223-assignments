/*
Name: Jonathan Hill
Date: April 12, 2023
Description: buddhabrot.c computes a program that outputs 
a PPM image of the Buddhabrot using multiple threads.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdbool.h>
#include <math.h>
#include "read_ppm.h"
#include "write_ppm.h"

pthread_mutex_t mutex;
pthread_barrier_t mybarrier;
float timer;

struct thread_info{
  int myid;
  int size;
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int *membership;
  int *count;
  int *max_count;
  struct ppm_pixel *picture;
  struct ppm_pixel *color_palette;
};

void *start(void *data){
  struct thread_info *buddha_data = (struct thread_info *)data;
  // perform step 1
  for (int i = buddha_data->row_start; i < buddha_data->row_end; i++){
    for (int j = buddha_data->col_start; j < buddha_data->col_end; j++){
      
      float xfrac = (float)j / buddha_data->size;
      float yfrac = (float)i / buddha_data->size;
      float x0 = buddha_data->xmin + xfrac * (buddha_data->xmax - buddha_data->xmin);
      float y0 = buddha_data->ymin + yfrac * (buddha_data->ymax - buddha_data->ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < buddha_data->maxIterations && ((x * x + y * y) < 2 * 2)){
        float x_temp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = x_temp;
        iter++;
      }

      if (iter < buddha_data->maxIterations){
        buddha_data->membership[(i * buddha_data->size) + j] = 1;
      } // escaped

      else{
        buddha_data->membership[(i * buddha_data->size) + j] = 0;
      }
    }
  }

  // perform step 2

  for (int i = buddha_data->row_start; i < buddha_data->row_end; i++){
    for (int j = buddha_data->col_start; j < buddha_data->col_end; j++){
  
      if (buddha_data->membership[(i * buddha_data->size) + j] == 1){
        float xfrac = (float) j / buddha_data->size;
        float yfrac = (float) i / buddha_data->size;
        float x0 = buddha_data->xmin + xfrac * (buddha_data->xmax - buddha_data->xmin);
        float y0 = buddha_data->ymin + yfrac * (buddha_data->ymax - buddha_data->ymin);

        float x = 0;
        float y = 0;
        float xtmp;
        while (x * x + y * y < 2 * 2){
          xtmp = x * x - y * y + x0;
          y = 2 * x * y + y0;
          x = xtmp;
          int yrow = round(buddha_data->size * (y - buddha_data->ymin) /
         (buddha_data->ymax - buddha_data->ymin));
          int xcol = round(buddha_data->size * (x - buddha_data->xmin) / 
         (buddha_data->xmax - buddha_data->xmin));
          if (yrow < 0 || yrow >= buddha_data->size){
          continue;
          } 
          if (xcol < 0 || xcol >= buddha_data->size){
          continue;
          }
          pthread_mutex_lock(&mutex);
          buddha_data->count[(yrow * buddha_data->size) + xcol]++;
          if (buddha_data->count[yrow * buddha_data->size + xcol] > *(buddha_data->max_count)){
           *(buddha_data->max_count) = buddha_data->count[yrow * buddha_data->size + xcol];
          }
        pthread_mutex_unlock(&mutex);
        }
        
      }
    }
  }

  // use a thread barrier to wait for all threads to finish steps 1 and 2
  pthread_barrier_wait(&mybarrier);
  // perform step 3
  float gamma = 0.681;
  float factor = 1.0 / gamma;
  for (int i = buddha_data->row_start; i < buddha_data->row_end; i++){
    for (int j = buddha_data->col_start; j < buddha_data->col_end; j++){
      float value = 0;
      if (buddha_data->count[i * buddha_data->size + j] > 0){
        value = log(buddha_data->count[i * buddha_data->size + j]) / log(*(buddha_data->max_count));
        value = pow(value, factor);
      }
      buddha_data->picture[i * buddha_data->size + j].red = value * 255;
      buddha_data->picture[i * buddha_data->size + j].green = value * 255;
      buddha_data->picture[i * buddha_data->size + j].blue = value * 255;
      
    }
  }
  return NULL;
}
int main(int argc, char *argv[]){
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  int max_count = 0;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1){
    switch (opt){
    case 's':
      size = atoi(optarg);
      break;
    case 'l':
      xmin = atof(optarg);
      break;
    case 'r':
      xmax = atof(optarg);
      break;
    case 't':
      ymax = atof(optarg);
      break;
    case 'b':
      ymin = atof(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> "
             "-b <ymin> -t <ymax> -p <numProcesses>\n",
             argv[0]);
      break;
    }
  }
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image

  pthread_t *num_threads = malloc(numProcesses * sizeof(pthread_t));
  struct thread_info *threads = malloc(numProcesses * sizeof(struct thread_info));
  int *membership = malloc(size * size * sizeof(int));
  int *count = malloc(size * size * sizeof(int));

  for(int i = 0; i < (size * size); i++){
    count[i] = 0;
  }

  struct ppm_pixel *picture;
  picture = (struct ppm_pixel *)malloc(size * size * sizeof(struct ppm_pixel));

  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&mybarrier,NULL,numProcesses);

  for (int i = 0; i < numProcesses; i++){
    threads[i].myid = i;
    threads[i].maxIterations = maxIterations;
    threads[i].size = size;
    threads[i].xmin = xmin;
    threads[i].xmax = xmax;
    threads[i].ymin = ymin;
    threads[i].ymax = ymax;
    threads[i].picture = picture;
    threads[i].membership = membership;
    threads[i].count = count;
    threads[i].max_count = &max_count;
    
    if (i == 0){
      threads[i].row_start = 0;
      threads[i].row_end = 240;
      threads[i].col_start = 0;
      threads[i].col_end = 240;
      int start_row = threads[i].row_start;
      int end_row = threads[i].row_end;
      int start_col = threads[i].col_start;
      int end_col = threads[i].col_end;
      printf("Thread %d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", i,
             start_col, end_col, start_row, end_row);
    }
    else if (i == 1){
      threads[i].row_start = 0;
      threads[i].row_end = 240;
      threads[i].col_start = 240;
      threads[i].col_end = 480;
      int start_row = threads[i].row_start;
      int end_row = threads[i].row_end;
      int start_col = threads[i].col_start;
      int end_col = threads[i].col_end;
      printf("Thread %d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", i,
             start_col, end_col, start_row, end_row);
    }
    else if (i == 2){
      threads[i].row_start = 240;
      threads[i].row_end = 480;
      threads[i].col_start = 0;
      threads[i].col_end = 240;
      int start_row = threads[i].row_start;
      int end_row = threads[i].row_end;
      int start_col = threads[i].col_start;
      int end_col = threads[i].col_end;
      printf("Thread %d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", i,
             start_col, end_col, start_row, end_row);
    }
    else if (i == 3){
      threads[i].row_start = 240;
      threads[i].row_end = 480;
      threads[i].col_start = 240;
      threads[i].col_end = 480;
      int start_row = threads[i].row_start;
      int end_row = threads[i].row_end;
      int start_col = threads[i].col_start;
      int end_col = threads[i].col_end;
      printf("Thread %d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", i,
             start_col, end_col, start_row, end_row);
  }
  pthread_create(&num_threads[i], NULL, start, (void *)&threads[i]);
  }

  for (int i = 0; i < numProcesses; i++){
    pthread_join(num_threads[i], NULL);
    printf("Thread %d) finished\n", i);
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Computed buddhabrot set (%ix%i) in %f seconds\n", size, size, timer);
  char file_name[160];
  sprintf(file_name, "buddhabrot-%i-%li.ppm", size, time(0));
  write_ppm(file_name, picture, size, size);
  printf("Writing file: %s\n", file_name);

  pthread_barrier_destroy(&mybarrier);
  pthread_mutex_destroy(&mutex);

  free(picture);
  free(num_threads);
  free(threads);
  free(membership);
  free(count);
}

