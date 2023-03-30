/*
Name: Jonathan Hill
Date: March 29, 2023
Description: multi_mandelbrot.c computes a program that
outputs a PPM image of the mandelbrot set using multiple
processes spawned with fork().
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char *argv[]){
  int size = 800;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  int child_status;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1)
  {
    switch (opt)
    {
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
  printf(" Num processes = %d\n", numProcesses);
  printf("X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  int shmid;
  struct ppm_pixel *picture;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
  if (shmid == -1)
  {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  picture = shmat(shmid, NULL, 0);
  if (picture == (void *)-1)
  {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  // generate pallet
  pid_t pid;
  float timer;

  srand(time(0));
  int shmid2;
  struct ppm_pixel *color_palette;
  shmid2 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * maxIterations, 0644 | IPC_CREAT);
  if (shmid2 == -1)
  {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }
  color_palette = shmat(shmid2, NULL, 0);
  if (color_palette == (void *)-1)
  {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  for (int i = 0; i < maxIterations; i++)
  {
    color_palette[i].red = rand() % 256;
    color_palette[i].green = rand() % 256;
    color_palette[i].blue = rand() % 256;
  }

  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  int row_start = 0;
  int row_end = 0;
  int col_start = 0;
  int col_end = 0;
  for (int i = 0; i < 4; i++){
    pid = fork();
    if (pid == 0){
      if (i == 0){
         row_start = 0;
         row_end = 240;
         col_start = 0;
         col_end = 240;
        printf("%d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", getpid(),
               col_start, col_end, row_start, row_end);
      }
      else if (i == 1){
        row_start = 0;
        row_end = 240;
        col_start = 240;
        col_end = 480;
        printf("%d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", getpid(),
               col_start, col_end, row_start, row_end);
      }
      else if (i == 2){
        row_start = 240;
        row_end = 480;
        col_start = 0;
        col_end = 240;
        printf("%d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", getpid(),
               col_start, col_end, row_start, row_end);
      }
      else if (i == 3){
        row_start = 240;
        row_end = 480;
        col_start = 240;
        col_end = 480;
        printf("%d) Sub-image block: cols (%i, %i) to rows (%i, %i)\n", getpid(),
               col_start, col_end, row_start, row_end);
      }
    for (int i = row_start; i < row_end; i++){
      for (int j = col_start; j < col_end; j++){
        float xfrac = (float) j / size;
        float yfrac = (float) i / size;
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin);

        float x = 0;
        float y = 0;
        int iter = 0;
        while (iter < maxIterations && ((x * x + y * y) < 2 * 2)){
          float x_temp = x * x - y * y + x0;
          y = 2 * x * y + y0;
          x = x_temp;
          iter++;
        }
        if (iter < maxIterations){
          picture[(i * size) + j] = color_palette[iter];
        }
        else{
          picture[(i * size) + j].red = 0;
          picture[(i * size) + j].blue = 0;
          picture[(i * size) + j].green = 0;
        }
      }
    }
    exit(0);
    }
  }
  for (int i = 0; i < numProcesses; i++){
    pid = wait(&child_status);
    printf("Child process complete: %d\n" , pid);
  }
  
    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
    printf("Computed mandelbrot set (%ix%i) in %f seconds\n", size, size, timer);
    char file_name[160];
    sprintf(file_name, "mandelbrot-%i-%li.ppm", size, time(0));
    write_ppm(file_name, picture, size, size);
    printf("Writing file: %s\n", file_name);

    if (shmdt(picture) == -1){
      perror("Error: cannot detatch from shared memory\n");
      exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1){
      perror("Error: cannot remove shared memory\n");
      exit(1);
    }
    if (shmdt(color_palette) == -1){
      perror("Error: cannot detatch from shared memory\n");
      exit(1);
    }

    if (shmctl(shmid2, IPC_RMID, 0) == -1){
      perror("Error: cannot remove shared memory\n");
      exit(1);
    }
  
}
