/*----------------------------------------------
 * Author: Jonathan Hill
 * Date: 
 * Description
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
