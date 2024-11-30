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

typedef struct GenHash {
  Node **head;
  int size;
} HashTable;

HashTable *createHashTable(int size) {
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  if (table == NULL) {
    return NULL;
  }
  table->size = size;
  table->head = (Node **)calloc(size, sizeof(Node *));
  if (table->head == NULL) {
    free(table);
    return NULL;
  }
  return table;
}

// Funcion para calcular el hash de un genero
int hashgen(const char *gene, int sizetable) {
  int hash = 0;
  for (int i = 0; gene[i] != '\0'; i++) {
    hash = (hash * 31 + gene[i]) % sizetable;
  }
  return hash;
}

Movie *input_movie(const char *filename, int *nmovies) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file %s\n", filename);
    return NULL;
  }
  Movie *movies = NULL;
  *nmovies = 0;
  char line[260];
  while (fgets(line, sizeof(line), file) != NULL) {
    (*nmovies)++;
    movies = (Movie *)realloc(movies, (*nmovies) * sizeof(Movie));
    if (movies == NULL) {
      printf("Error reallocating memory\n");
      fclose(file);
      return NULL;
    }
    sscanf(line, "%[^,],%[^,],%f,%d", (movies[*nmovies - 1].title),
           (movies[*nmovies - 1].genre), &(movies[*nmovies - 1].rating),
           &(movies[*nmovies - 1].time));
  }
  fclose(file);
  return movies;
}

//intercambiar peliculas
void swap(Movie *a, Movie *b) {
  Movie temp = *a;
  *a = *b;
  *b = temp;
}

//particio para el quicksort
int partition(Movie arr[], int low, int high) {
  char *pivot = arr[high].genre;
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (strcmp(arr[j].genre, pivot) < 0) {  // Orden lexicográfico
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return i + 1;
}

//quicksort
void quicksort(Movie arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quicksort(arr, low, pi - 1);
    quicksort(arr, pi + 1, high);
  }
}

//funcion que agrupa las peliculas por genero con quicksort
void group_by_genre(Movie movies[], int nmovies) {
  // Ordenar las películas por género
  quicksort(movies, 0, nmovies - 1);

  //agrupa e imprime
  printf("Peliculas agrupadas por genero:\n");
  char current_genre[50] = "";
  for (int i = 0; i < nmovies; i++) {
    if (strcmp(current_genre, movies[i].genre) != 0) {
      //nuevo gen encontrado
      strcpy(current_genre, movies[i].genre);
      printf("\nGenero: %s\n", current_genre);
    }
    printf("  Titulo: %s, Rating: %f, Duracion: %d\n",
           movies[i].title, movies[i].rating, movies[i].time);
  }
}

int main() {
  int nmovies;
  Movie *movies = input_movie("peliculas.txt", &nmovies);
  if (movies == NULL) {
    printf("Error reading movies\n");
    return 1;
  }

  printf("%s %s %f %d", movies[0].title, movies[0].genre, movies[0].rating,movies[0].time);
  printf("\n%s %s %f %d", movies[15].title, movies[15].genre, movies[15].rating,movies[15].time);

  //prueba de buscar por generos
  group_by_genre(movies, nmovies);
  free(movies);


  return 0;
}