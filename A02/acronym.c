#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
  printf("Enter a phrase: \n");
  char phrase[1024];
  char new_acronym[1024];
  char *index = new_acronym;
  scanf("%[^\n]%*c", phrase);
  int phrase_length = strlen(phrase);
  for (int i = 0; i < phrase_length; i++){
    if (64 < phrase[i] && phrase[i] < 90){
      *index = phrase[i];
       index = index + 1;
     }
   }
    *index = '\0';
    printf("Your phrase is: %s\n", new_acronym);
  return 0;
}
