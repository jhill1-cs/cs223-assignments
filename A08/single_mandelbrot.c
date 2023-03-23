Name: Jonathan Hill
Date: Mar. 22, 2023
Description: single_mandelbrot.c computes a program that
 outputs a PPM image of the mandelbrot set

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  struct ppm_pixel* picture;
  picture = (struct ppm_pixel*)malloc(size * size * sizeof(struct ppm_pixel));
  // for (int i = 0; i < size; i++) {
  //   picture[i] = (struct ppm_pixel*)malloc(size * sizeof(struct ppm_pixel));
  // }

  // generate pallet
  srand(time(0));
  struct ppm_pixel* color_palette;
  color_palette = (struct ppm_pixel*)malloc((maxIterations) * sizeof(struct ppm_pixel));

  for (int i = 0; i < maxIterations; i++) {
     color_palette[i].red = rand() % 256;
     color_palette[i].green = rand() % 256;
     color_palette[i].blue = rand() % 256;
  }


  float timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);



  // compute image
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){
      float xfrac = (float) j / size;
      float yfrac = (float) i / size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < maxIterations && (x*x + y*y < 2*2)){
        float x_temp = x*x - y*y + x0;
        y = 2*x*y + y0;
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
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%ix%i) in %f seconds\n", size, size, timer);
  char file_name[160];
  sprintf(file_name, "mandelbrot-%i-%li.ppm" , size, time(0)); 
  write_ppm(file_name, picture , size, size);
  free(picture);
  free(color_palette);

}


