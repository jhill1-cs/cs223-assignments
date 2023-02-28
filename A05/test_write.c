/*----------------------------------------------
 * Author: Jonathan Hill
 * Date:
 * Description: This program reads in feep-raw.ppm
and writes it to test.ppm. It then reads in test.ppm
and prints the pixel values.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // test writing the file to test.ppm, reload it, and print the contents

  write_ppm("test.ppm", pixels, w, h);
  free(pixels);

  struct ppm_pixel* test_pixels = read_ppm("test.ppm", &w, &h);

  for (int i = 0; i < (w * h); i++) {
    printf("(%d, %d, %d) ", test_pixels[i].red, test_pixels[i].green, test_pixels[i].blue);
  }

  free(test_pixels);
  free(pixels);

  return 0;
}
