/*----------------------------------------------
 * Author: Jonathan Hill
 * Date: Feb. 15, 2023
 * Description: This program takes in a csv file of songs
and stores them in a linked list, locates and prints the 
song with the highest danceability, and removes this song
from the list. The new list is displayed and the process
repeats until all of the songs are removed..
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct song_info{
    int min;
    int sec;
    float danceability;
    float energy;
    float tempo;
    float valence;
    char artist[128];
    char title[128];
};

struct node{
    struct song_info *song;
    struct node *next;
};

struct node *insertSong(struct song_info *song, struct node *head){
    struct node *n = malloc(sizeof(struct node));
    if (n == NULL)
    {
        printf("ERROR: Out of space!\n");
        exit(1);
    }
    n->song = song;
    n->next = head;
    head = n;
    return n;
}

struct node *removeSong(struct node *head, struct node *maxDanceability) {
    if (head == NULL){
        return NULL;
    }
    if (head->song->danceability == maxDanceability->song->danceability) {
        struct node *temp = head->next;
        free(head->song);
	free(head);
        return temp;
    } 
    else {
        struct node *temp = head->next;
        struct node *prev = head;
        while (temp != NULL && (temp->song->danceability != maxDanceability->song->danceability)) {
            prev = temp;
            temp = temp->next;
        } 
        if (temp == NULL) {
            return head;
        }
        else{
        prev->next = temp->next;
	free(temp->song);
        free(temp);
        return head;
    }
}
}


struct node *maxDanceability(struct node *head){
    struct node *dance_node = head;
    float max_dance = head->song->danceability;
    struct node *temp = head->next;
    while(temp!= NULL){
        if(temp->song->danceability > max_dance){
            max_dance = temp->song->danceability;
            dance_node = temp;
        }
        temp = temp->next;
    }
    return dance_node;
}
void clearList(struct node *head){
    struct node *current = head;
    while (current != NULL){
        head = head->next;
        free(current);
        current = head;
    }
}

void printSong(struct node *head){
    int index = 0;
    while (head != NULL){
        printf("%i) %-20s artist: %-20s duration: %d:%d  D: %f E: %f\n T:%f  V:%f\n",
               index, head->song->title, head->song->artist, head->song->min, head->song->sec,
               head->song->danceability, head->song->energy, head->song->tempo, head->song->valence);
        head = head->next;
        index++;
    }
}
int convertMin(int a){
    int min = (a / 1000) / 60;
    return min;
}
int convertSec(int a){
    int sec = (a / 1000) % 60;
    return sec;
}

int main(int argc, char *argv[]){
    FILE *infile;
    infile = fopen("songlist.csv", "r"); // r means read in the file
    char row[1000];
    char *tokens[8];
    fgets(row, 1000, infile);

    struct node *head = NULL;

    while(fgets(row, 1000, infile)!=NULL){
        char *token = strtok(row, ",");
        int index = 0;
        while (token != NULL){
            tokens[index] = token;
            token = strtok(NULL, ",");
            index++;
        }
 
    struct song_info *song = (struct song_info *)malloc(sizeof(struct song_info));
    strcpy(song->title, tokens[0]);
    strcpy(song->artist, tokens[1]);
    song->min = convertMin(atoi(tokens[2]));
    song->sec = convertSec(atof(tokens[2]));
    song->danceability = atof(tokens[3]);
    song->energy = atof(tokens[4]);
    song->tempo = atof(tokens[5]);
    song->valence = atof(tokens[6]);

    head = insertSong(song, head);
    }
    printSong(head);

    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    char user_input;
    scanf(" %c", &user_input);
    while (user_input == 'd'){ 
      if (head != NULL){
        struct node *maxDance = maxDanceability(head);
        printf("Most danceable: %-20s artist: %-20s duration: %d:%d  D: %f E: %f\n T:%f  V:%f\n",
               maxDance->song->title, maxDance->song->artist, maxDance->song->min, maxDance->song->sec,
               maxDance->song->danceability, maxDance->song->energy, maxDance->song->tempo, maxDance->song->valence);
        head = removeSong(head, maxDance);
                printSong(head);
        printf("Press 'd' to show the most danceable song (any other key to quit): ");
        scanf(" %c", &user_input);
      }
      else{
      printf("Dataset contains 0 songs.");
      break;
      }
    }
    
    clearList(head);
    fclose(infile);
    return 0;
}


