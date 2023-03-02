/*----------------------------------------------
 * Author: Jonathan Hill
 * Date: Mar. 1, 2023
 * Description:This program reads in a PPM file
and ouputs the dimensions, the maximum number of
characters it can store, and the secret message
 stored in the least significant bits of each color.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  else{
    int width;
    int height;
    struct ppm_pixel* pixel = read_ppm(argv[1], &width, &height);
    printf("width: %d, height: %d\n" , width, height);

    if (!pixel){
      printf("Error.\n");
    }
    printf("Reading %s with width %d and height %d\n", argv[1], width, height);
    int max_char = (width * height * 3) / 8;
    int message_index = 0;
    char* message = NULL;
    int message_size = 8;
    int counter = 0;
    int index = 0;
    message = (char* )malloc((max_char + 1)*sizeof(char)); 
    message[message_index] = '\0';
    unsigned char message_bit = 0;

    for (int i = 0; i < (width * height); i++) {
        if (counter < 8) {
          unsigned char val = pixel[i].red & 0x1;
          val = val << (7 - counter);
          message_bit = message_bit | val;
          counter++;
        }
        if (counter == message_size) {
          message[index] = message_bit;
          index++;
          counter = 0; 
          message_bit = 0;
        }

        if (counter < 8) {
          unsigned char val = pixel[i].green & 0x1;
          val = val << (7 - counter);
          message_bit = message_bit | val;
          counter++;
        }
        if (counter == message_size) {
          message[index] = message_bit;
          index++;
          counter = 0; 
          message_bit = 0;
        }

        if (counter < 8) {
          unsigned char val = pixel[i].blue & 0x1;
          val = val << (7 - counter);
          message_bit = message_bit | val;
          counter++;
        }
        if (counter == message_size) {
          message[index] = message_bit;
          index++;
          counter = 0; 
          message_bit = 0;
        }



    }
    printf("Max number of characters in the image: %d\n", max_char);

    for (int j = 0; j < strlen(message); j++) {
      if (message[j] == '\0') {
        break;
      } 
      printf("%c", message[j]);
    }

    free(message);
    free(pixel);
  }
  

  return 0;
}

