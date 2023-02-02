/**************************************************
 * mathgame.c
 * Author:Jonathan Hill
 * Implements a math game
 */

#include <stdio.h>

int main() { 
  printf("Welcome to math games!\n");
  printf("How many rounds do you want to play?\n");
  int input;
  scanf("%d", &input);
  int correct_count=0;
  int answer;
  for (int i = 0; i< input; i++) {
    int first_random_num = rand() % 9 + 1;
    int second_random_num = rand() % 9 + 1;
    int sum = first_random_num + second_random_num;
    printf("%d + %d = ?\n" , first_random_num , second_random_num);
    scanf("%d", &answer);
    if (answer == sum){
      correct_count++;
      printf("Correct!\n");
    }
    else{
      printf("Incorrect :(\n");
    }
  }
  printf("You answered %d/%d correctly.\n", correct_count, input);
  return 0;
}


