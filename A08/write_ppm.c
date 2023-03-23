#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* fp;
    fp = fopen(filename, "wb");

    fprintf(fp, "P6\n%d %d\n%d\n", w, h, 255);
    fwrite(pixels, sizeof(struct ppm_pixel), (w*h), fp);
    fclose(fp);

}
void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
}
