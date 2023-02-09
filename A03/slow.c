/*----------------------------------------------
 * Author: Jonathan Hill
 * Date:Feb. 8, 2023 
 * Description: This code takes in a user pause and user text.
It then mimicks entwise speech by taking in the user pause input,
 uses the stored user pause value as the number of dots needed in
 between each char, and outputting the final string.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char user_text[32];
  int user_pause;
  printf("Pause length: ");
  scanf("%d" , &user_pause);
  printf("Text: ");
  scanf("%s" , user_text);

  char* new_string;
  new_string = (char*)malloc(strlen(user_text) + (strlen(user_text)*user_pause) +1);
  int index = 0;
  for (int i = 0; i < strlen(user_text); i++){
    new_string[index++] = user_text[i];
    for(int j = 0; j < user_pause; j++){
      new_string[index++] = '.';
    }
  }
  new_string[index] = '\0';
  printf("%s\n", new_string);
  free(new_string);
  return 0;
}
