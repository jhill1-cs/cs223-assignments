/*----------------------------------------------
 * Author: Jonathan Hill
 * Date: Feb. 08, 2023
 * Description: This code reads in a csv file and outputs the rows of song
information into an "attractive" table organized by artist, title,
duration, danceability, energy, tempo, and valence.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct song_info {
  int min;
  int sec;
  float danceability;
  float energy;
  float tempo;
  float valence;
  char artist[128];
  char title[128];
};
void printSong(struct song_info songs[], int length) {
  for (int i = 0; i < 18; i++){
    printf("%i) %-20s artist: %-20s duration: %d:%d  D: %f E: %f\n T:%f  V:%f\n" ,i , songs[i].title , 
    songs[i].artist, songs[i].min, songs[i].sec , songs[i].danceability, songs[i].energy, 
    songs[i].tempo, songs[i].valence);
  }
}
int convertMin(int a){
  int min = (a/1000) / 60;
  return min;
}
int convertSec(int a){
  int sec = (a/1000) % 60;
  return sec;
}

int main (int argc, char *argv[]) { 
    // struct song_info* songs; 
    // songs = malloc(sizeof(char));
    // int num_rows = 18;
    FILE *infile; 
    infile = fopen("songlist.csv", "r"); // r means read in the file
    char row[1000];
    char* tokens[8];
    fgets(row, 1000, infile);
    strtok(row, ",");
    int num_rows = atoi(row);
    printf("%d", num_rows);

    fgets(row, 1000, infile);
    struct song_info *songs_array;
    songs_array = (struct song_info*)malloc(sizeof(struct song_info) * (num_rows + 1));
 
    int index2 = 0;
    while(!feof(infile)){
      fgets(row, 1000, infile);
      // printf("Row: %s", row);
      char* token = strtok(row, ",");
      int index = 0 ;
      while (token != NULL){
        // printf("Token: %s\n", token);
        tokens[index] = token;
        token = strtok(NULL, ",");
        index++;
      }

     strcpy(songs_array[index2].title, tokens[0]);
     strcpy(songs_array[index2].artist, tokens[1]);
     songs_array[index2].min = convertMin(atoi(tokens[2]));
     songs_array[index2].sec = convertSec(atof(tokens[2]));
     songs_array[index2].danceability = atof(tokens[3]);
     songs_array[index2].energy = atof(tokens[4]);
     songs_array[index2].tempo = atof(tokens[5]);
     songs_array[index2].valence = atof(tokens[6]);
     index2++;
    }
   printSong(songs_array, num_rows);
   free(songs_array);
   fclose(infile);
   return 0;
}
  
