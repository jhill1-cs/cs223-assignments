/*
Name: Jonathan Hill
Date: April 5, 2023
Description: thread_mandelbrot.c computes a
program that outputs a PPM image of a mandelbrot set
using threads.
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
#include "read_ppm.h"
#include "write_ppm.h"

pthread_mutex_t mutex;
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
  struct ppm_pixel *picture;
  struct ppm_pixel *color_palette;
};

void *mandelbrot_thread(void *data){
struct thread_info *mandel_data = (struct thread_info *)data;
for (int i = mandel_data->row_start; i < mandel_data->row_end; i++){
  for (int j = mandel_data->col_start; j < mandel_data->col_end; j++){
    float xfrac = (float)j / mandel_data->size;
    float yfrac = (float)i / mandel_data->size;
    float x0 = mandel_data->xmin + xfrac * (mandel_data->xmax - mandel_data->xmin);
    float y0 = mandel_data->ymin + yfrac * (mandel_data->ymax - mandel_data->ymin);

    float x = 0;
    float y = 0;
    int iter = 0;
    while (iter < mandel_data->maxIterations && ((x * x + y * y) < 2 * 2)){
      float x_temp = x * x - y * y + x0;
      y = 2 * x * y + y0;
      x = x_temp;
      iter++;
    }
    if (iter < mandel_data->maxIterations){
      mandel_data->picture[(i * mandel_data->size) + j] = mandel_data->color_palette[iter];
    }
    else{
      mandel_data->picture[(i * mandel_data->size) + j].red = 0;
      mandel_data->picture[(i * mandel_data->size) + j].blue = 0;
      mandel_data->picture[(i * mandel_data->size) + j].green = 0;
    }
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

  pthread_t *num_threads = malloc(numProcesses * sizeof(pthread_t));
  struct thread_info *threads = malloc(numProcesses * sizeof(struct thread_info));

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
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here

  struct ppm_pixel* picture;
  picture = (struct ppm_pixel*)malloc(size * size * sizeof(struct ppm_pixel));

  // generate pallet
  srand(time(0));
  struct ppm_pixel* color_palette;
  color_palette = (struct ppm_pixel*)malloc((maxIterations) * sizeof(struct ppm_pixel));
  for (int i = 0; i < maxIterations; i++) {
     color_palette[i].red = rand() % 256;
     color_palette[i].green = rand() % 256;
     color_palette[i].blue = rand() % 256;
  }

  // compute image
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  pthread_mutex_init(&mutex, NULL);
  for (int i = 0; i < numProcesses; i++){
    threads[i].myid = i;
    threads[i].maxIterations = maxIterations;
    threads[i].size = size;
    threads[i].xmin = xmin;
    threads[i].xmax = xmax;
    threads[i].ymin = ymin;
    threads[i].ymax = ymax;
    threads[i].color_palette = color_palette;
    threads[i].picture = picture;
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
  pthread_create(&num_threads[i], NULL, mandelbrot_thread, (void *)&threads[i]);
  }

  for (int i = 0; i < numProcesses; i++){
    pthread_join(num_threads[i], NULL);
    printf("Thread %d) finished\n", i);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Computed mandelbrot set (%ix%i) in %f seconds\n", size, size, timer);
  char file_name[160];
  sprintf(file_name, "mandelbrot-%i-%li.ppm", size, time(0));
  write_ppm(file_name, picture, size, size);
  printf("Writing file: %s\n", file_name);

  pthread_mutex_destroy(&mutex);

  free(num_threads);
  free(threads);
  free(color_palette);
  free(picture);
}
