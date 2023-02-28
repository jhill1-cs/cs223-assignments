/*----------------------------------------------
 * Author:Jonathan Hill
 * Date:
 * Description: This program prints out the pixel values
of feep-raw.ppm.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // todo: print out the grid of pixels
  for (int i = 0; i < (w * h); i++) {
     printf("(%d, %d, %d)\n", pixels[i].red, pixels[i].green, pixels[i].blue);
  }
  free(pixels);
  return 0;
}

