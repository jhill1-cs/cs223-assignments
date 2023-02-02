#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song_info {
  int minutes;
  int seconds;
  float danceability;
  char artist[32];
  char title[32];
};
void printSong(struct song_info songs[], int length) {
  for (int i = 0; i < length; i++){
    printf("%-20s artist: %-20s duration: %d:%-20d danceability: %f\n" , songs[i].title , songs[i].artist, songs[i].minutes, songs[i].seconds, songs[i].danceability);
  }
}
int main() {
  printf("Welcome to Steven Struct's Song List.\n");
  struct song_info songs [3];
  songs[0].minutes = 3;
  songs[0].seconds = 49;
  songs[0].danceability = .8;
  strcpy(songs[0].artist, "Sevdaliza");
  strcpy(songs[0].title, "High Alone");
  songs[1].minutes = 3;
  songs[1].seconds = 54;
  songs[1].danceability = .6;
  strcpy(songs[1].artist, "Danger Mouse");
  strcpy(songs[1].title, "Belize");
  songs[2].minutes = 3;
  songs[2].seconds = 7;
  songs[2].danceability = .7;
  strcpy(songs[2].artist, "SiR");
  strcpy(songs[2].title, "John Redcorn");

  int user_id;
  printf("Enter a song id to edit [0,1,2]:\n");
  scanf("%d" , &user_id);
  if (user_id!=0 && user_id!=1 && user_id!=2){
    printf("Invalid choice!\n");
  }
  else{
      char user_attribute[32];
      printf("Which attribute do you wish to edit? [artist, title, duration, danceability]:\n");
      scanf("%s" , user_attribute);
      // int user_artist = strcmp(user_attribute, "artist");
      // int user_title = strcmp(user_attribute, "title");
      // int user_duration = strcmp(user_attribute, "duration");
      // int user_danceability = strcmp(user_attribute, "danceability");
      char input[32];
      
        if (strcmp(user_attribute, "artist")==0){
          printf("Enter artist: \n");
          scanf("%s" , input);
          strcpy(songs[user_id].artist, input); 
        }
          else if(strcmp(user_attribute, "title")==0){
          printf("Enter title: \n"); 
          scanf("%s" , input);
          strcpy(songs[user_id].title, input);
          }
          else if(strcmp(user_attribute, "duration")==0){
            int user_min;
            int user_sec;
            printf("Enter duration: \n");
            scanf("%d:%d" , &user_min, &user_sec);
            songs[user_id].minutes = user_min;
            songs[user_id].seconds = user_sec;
          }
          else if(strcmp(user_attribute, "danceability")==0){
            float user_dance;
            printf("Enter danceability: \n");
            scanf("%f" , &user_dance);
            songs[user_id].danceability = user_dance;
            }
       printSong(songs, 3);   
}
  return 0;
}
