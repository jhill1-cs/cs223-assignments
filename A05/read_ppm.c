/*----------------------------------------------
 * Author: Jonathan Hill
 * Date:
 * Description:This program reads in a PPM file 
stored in binary format and returns the width and
the height stored in the file..
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *fp;
  char line[1024];
  fp = fopen(filename, "rb");

  if(!fp){
   return NULL;
  }

  fgets(line, sizeof(line), fp);
  fgets(line, sizeof(line), fp);
  fgets(line, sizeof(line), fp);
  sscanf(line, "%d %d" , w, h);
  printf("Width: %d\n" , *w);
  printf("Height: %d\n", *h);
  fgets(line, sizeof(line), fp);

  struct ppm_pixel* picture = (struct ppm_pixel*)malloc(*w * *h * sizeof(struct ppm_pixel));
  if (!picture){
    fclose(fp);
    return NULL;
  }
  fread(picture, sizeof(struct ppm_pixel), *w * *h, fp);
  fclose(fp);
  
  return picture;
}


struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}



