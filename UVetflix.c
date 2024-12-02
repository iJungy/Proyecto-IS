#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_GEN 50

typedef struct Movie {
  char title[MAX_TITLE];
  char genre[MAX_GEN];
  float rating;
  int time;
} Movie;

typedef struct Node {
  Movie *movie;
  struct Node *next;
} Node;

// cargar peliculas del archivo
void loadmovies(const char *filename, Movie **movie, int cantidad){
  FILE *file = fopen(filename,"r");
}

