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
//--------------Funciones para agrupar peliculas por genero----------------------------
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
    printf("  Titulo: %s, Rating: %.2f, Duracion: %d\n",
           movies[i].title, movies[i].rating, movies[i].time);
  }
}
//-------------------------------------------------------------------------------

// ----------------------------TOP 10------------------------------------------------
// ----------------------------HEAPSORT----------------------------------
void heap(Movie arr[],int n, int  i) {
  int large = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && arr[left].rating > arr[large].rating)
    large = left;

  if (right < n && arr[right].rating > arr[large].rating)
    large = right;

  if (large!= i) {
    swap(&arr[i], &arr[large]);
    heap(arr, n, large);
  }
}

void heapSort(Movie arr[], int n) {
  for (int i = n / 2 - 1; i >= 0; i--)
      heap(arr, n, i);

  for (int i=n-1; i>0; i--)
    {
      swap(&arr[0], &arr[i]);
      heap(arr, i, 0);
    }
}

void showtop10(Movie movie[], int num_movies) {
  printf("\nTop 10 peliculas:\n");
  heapSort(movie, num_movies);
  for (int i = 0; i < 10; i++) {
    printf("Titulo: %s, Rating: %.2f, Duracion: %d\n", movie[i].title, movie[i].rating, movie[i].time);
  }
}

// ----------------------------------------------------------------------------

// --------------- Peliculas por estrellas ------------------------------------

void countingsortrating(Movie movies[],int num_movies) {
  int max = 5;
  int min = 1;
  int rango = max - min +1;

  // contador para la calificacion
  int count[rango];
  memset(count, 0, sizeof(count));

  // Array para almacenar los resultados ordenados
  Movie *rs = (Movie *)malloc(num_movies * sizeof(Movie));
  if (rs == NULL) {
    printf("Error al asignar memoria\n");
    return;
  }

  // contar las veces que aparecen la calificacion
  for (int i= 0; i<num_movies; i++) {
    count[movies[i].rating - min]++;
  }

  // calcular las posiciones acumuladas
  for (int i=1;i<rango;i++ ) {
    count[i] += count[i-1];
  }

  // construimos de nuevo el array ordenado
  for (int i = num_movies -1 ;i>= 0;i--) {
    int position = (int)movies[i].rating - min;
    rs[count[position]-1] = movies[1];
    count[position]--;
  }

  // copiamos el resultado al arreglo original
  for (int i =0; i<num_movies; i++) {
    movies[i] = rs[i];
  }

  free(rs);

  printf("\nPeliculas por estrellas:\n");
  for (int i = 0; i < num_movies; i++) {
    printf("Titulo: %s, Rating: %.2f, Duracion: %d\n", movies[i].title, movies[i].rating, movies[i].time);
  }

}
// ------------------------------------------------------------------------------
int main() {
  int nmovies;
  Movie *movies = input_movie("peliculas.txt", &nmovies);
  if (movies == NULL) {
    printf("Error reading movies\n");
    return 1;
  }

  int x;
  printf("\n+--------------------------------------------------------------+\n");
  printf("|                       Bienvenido a UVetflix                       |\n");
  printf("+--------------------------------------------------------------+");
  do {
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                          MENU PRINCIPAL                      |\n");
    printf("+--------------------------------------------------------------+\n");
    printf("| 1. Mostrar peliculas por genero                              |\n");
    printf("| 2. Mostrar top 10                                            |\n");
    printf("| 3. Calcular cuantas peliculas puede ver en el tiempo deseado |\n");
    printf("| 4. Buscar pelicula                                           |\n");
    printf("| 5. Ver primer pelicula en el historial                       |\n");
    printf("| 6. Ver todo el historial de peliculas vistas                 |\n");
    printf("| 7. Salir                                                     |\n");
    printf("+--------------------------------------------------------------+\n");
    printf("Elija la opcion deseada: ");
    scanf("%d", &x);
    getchar();

    switch (x) {
      case 1: {
        //prueba de ordenar por generos
        group_by_genre(movies, nmovies);
        break;
      }
      case 2: {
        printf("%s %s %f %d", movies[0].title, movies[0].genre, movies[0].rating,movies[0].time);
        printf("\n%s %s %f %d", movies[15].title, movies[15].genre, movies[15].rating,movies[15].time);
        //ToDo: Mostrar top 10
        break;
      }
      case 3: {
        //ToDo: Calcular peliculas en t
        break;
      }
      case 4: {
        //ToDo: Buscar pelicula
        break;
      }
      case 5: {
        //ToDo: Ver primer pelicula en el historial
        break;
      }
      case 6: {
        //ToDo: Ver todo el historial
        break;
      }
      case 7:
        printf("Saliendo del programa...\n");
      break;
      default:
        printf("Opcion no valida.\n");
    }
  } while (x != 7);

  free(movies);

  return 0;
}