#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_GEN 50

typedef struct Movie{
    char title[MAX_TITLE];
    char genre[MAX_GEN];
    int rating;
    int time;
}Movie;

typedef struct Node{
Movie *movie;
struct Node *next;
}Node;

Movie *input_movie(const char *filename, int *nmovies){
  FILE *file = fopen(filename, "r");
  if (file == NULL){
    printf("Error opening file %s\n", filename);
    return NULL;
  }
  Movie *movies = NULL;
  *nmovies = 0;
  char line[260];
  while(fgets(line,sizeof(line),file) != NULL){
    (*nmovies)++;
    movies = (Movie*)realloc(movies,(*nmovies) * sizeof(Movie));
    if(movies == NULL){
      printf("Error reallocating memory\n");
      fclose(file);
      return NULL;
    }
    sscanf(line,"%[^,],%[^,],%d,%d",
      (movies[*nmovies - 1].title),
      (movies[*nmovies - 1].genre),
      &(movies[*nmovies - 1].rating),
      &(movies[*nmovies - 1].time));
  }
  fclose(file);
  return movies;
}



int main() {
  int nmovies;
  Movie *movies = input_movie("peliculas.txt", &nmovies);
  if(movies == NULL) {
    printf("Error reading movies\n");
    return 1;
  }

  free(movies);
  return 0;
}