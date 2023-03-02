/*----------------------------------------------
 * Author:Jonathan Hill
 * Date:Mar. 1, 2023
 * Description:This programs reads in a single 64
bit unsigned integer and outputs it as an 8x8
sprite, with "@" denoting 1 and " " denoting 0. 
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here
  unsigned long left_most = 0x1ul << 63;

  for (int i = 0; i < 8; i++){
    int row = i;
    for (int j = 0; j < 8; j++){
      int col = j;
      int bit_position = row * 8 + col;
      unsigned long mask = 1ul << (63-bit_position);
      unsigned long bit = (img & mask) >> (63- bit_position);

      if (bit){
        printf("@ ");
      }
      else{
        printf("  ");
      }
      left_most = left_most >> 1;
    }
    printf("\n");
  }

  return 0;
}
