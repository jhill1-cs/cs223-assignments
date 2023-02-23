/*----------------------------------------------
 * Author: Jonathan Hill
 * Date: Feb. 22, 2023
 * Description: This program takes in two user
arguments as inputs, finds the common letter index,
and prints the words in a crossword format.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  if (argc < 3){
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }
  char *user_word_1 = argv[1];
  int word_1_length = strlen(user_word_1);
  char *user_word_2 = argv[2];
  int word_2_length = strlen(user_word_2);
  int common_ind = -1;
  int common_ind_2 = -1;
  for (int i = 0; i < word_1_length; i++){
    for (int j = 0; j < word_2_length; j++){
      if (user_word_1[i] == user_word_2[j]) {
        common_ind = i;
        common_ind_2 = j;
        break;
   }
  }
    if (common_ind >= 0){
      break;
    }
  }
   if (common_ind < 0 ){
    printf("No common letter!\n");
    exit(1);
  }

    char **two_d_array = malloc(sizeof(char*) * word_2_length);
    for (int i = 0; i < word_2_length; i++) {
        two_d_array[i] = malloc(sizeof(char) * word_1_length);
        for (int j = 0; j < word_1_length; j++) {
            if (i == common_ind_2) {
                two_d_array[i][j] = user_word_1[j];
            } else if (j == common_ind) {
                two_d_array[i][j] = user_word_2[i];
            } else {
                two_d_array[i][j] = '.';
            }
        }
    }

    for (int i = 0; i < word_2_length; i++) {
        for (int j = 0; j < word_1_length; j++) {
            printf("%c ", two_d_array[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < word_2_length; i++) {
        free(two_d_array[i]);
    }
    free(two_d_array);

    return 0;
}
