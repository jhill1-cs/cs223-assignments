#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    char char_list[] = {'@', '!', '#', '*', '\0'};
    int char_list_length = strlen(char_list);
    printf("Enter a word: \n");
    char word[32];
    scanf("%s" , word);
    int word_length = strlen(word);
    for (int i = 0; i < word_length; i++){
        if (word[i] > 96){
            char random_char = char_list[rand() % char_list_length];      
            word[i] = random_char;
        }
    }
    printf("OMG! %s\n" , word);
  return 0;
}
