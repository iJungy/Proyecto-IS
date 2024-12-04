#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TITLE 100
#define MAX_GEN 50

#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define GREEN   "\x1b[32m"

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
//Se usa mergesort
//intercambiar películas
void swap(Movie *a, Movie *b) {
  Movie temp = *a;
  *a = *b;
  *b = temp;
}

//función auxiliar para fusionar dos mitades
void merge(Movie arr[], int left, int center, int right) {
  int n1 = center - left + 1;
  int n2 = right - center;

  //crear arrays temporales
  Movie *leftArr = (Movie *)malloc(n1 * sizeof(Movie));
  Movie *rightArr = (Movie *)malloc(n2 * sizeof(Movie));

  //copiar datos a los arrays temporales
  for (int i = 0; i < n1; i++) {
    leftArr[i] = arr[left + i];
  }
  for (int j = 0; j < n2; j++) {
    rightArr[j] = arr[center + 1 + j];
  }

  //mezclar las dos mitades ordenadas
  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (strcmp(leftArr[i].genre, rightArr[j].genre) <= 0) {
      arr[k] = leftArr[i];
      i++;
    } else {
      arr[k] = rightArr[j];
      j++;
    }
    k++;
  }

  //copiar los elementos restantes de ambas mitades
  while (i < n1) {
    arr[k] = leftArr[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = rightArr[j];
    j++;
    k++;
  }

  //liberar memoria de los arrays temporales
  free(leftArr);
  free(rightArr);
}

//implementación de MergeSort
void mergesort(Movie arr[], int left, int right) {
  if (left < right) {
    int center = left + (right - left) / 2;

    //dividir en mitades y ordenarlas recursivamente
    mergesort(arr, left, center);
    mergesort(arr, center + 1, right);

    // Fusionar las mitades ordenadas
    merge(arr, left, center, right);
  }
}

//función para agrupar las películas por género usando MergeSort
void group_by_genre(Movie movies[], int nmovies) {
  //ordenar las películas por género
  mergesort(movies, 0, nmovies - 1);

  //agrupar e imprimir
  printf("Peliculas agrupadas por genero:\n");
  char current_genre[50] = "";
  for (int i = 0; i < nmovies; i++) {
    if (strcmp(current_genre, movies[i].genre) != 0) {
      //nuevo género encontrado
      strcpy(current_genre, movies[i].genre);
      printf("\nGenero: %s\n", current_genre);
    }
    printf("  Titulo: %s, Rating: %.2f, Duracion: %d\n",
           movies[i].title, movies[i].rating, movies[i].time);
  }
}
//-------------------------------------------------------------------------------
//--------------Funciones para buscar pelicula----------------------------------
//Usando algoritmo KMP

//funcion para construir la tabla de prefijos del patron
void buildPrefixTable(const char *pattern, int *prefix, int m) {
  int j = 0;
  prefix[0] = 0;
  for (int i = 1; i < m; i++) {
    if (pattern[i] == pattern[j]) {
      j++;
      prefix[i] = j;
    } else {
      if (j != 0) {
        j = prefix[j - 1];
        i--; //retrocede y reintenta
      } else {
        prefix[i] = 0;
      }
    }
  }
}

//funcion KMP para buscar palabras clave en un texto
int KMP(const char *text, const char *pattern) {
  int n = strlen(text);
  int m = strlen(pattern);
  int *prefix = (int *)malloc(m * sizeof(int));
  if (prefix == NULL) {
    printf("Error allocating memory for prefix table\n");
    return 0;
  }

  buildPrefixTable(pattern, prefix, m);

  int i = 0, j = 0;
  while (i < n) {
    if (pattern[j] == text[i]) {
      i++;
      j++;
    }
    if (j == m) {
      free(prefix);
      return 1; //patron encontrado
    } else if (i < n && pattern[j] != text[i]) {
      if (j != 0) {
        j = prefix[j - 1];
      } else {
        i++;
      }
    }
  }

  free(prefix);
  return 0;//patron no encontrado
}
void search(Movie *movies, int nmovies, const char *keyword) {
  printf("Resultados para la palabra clave '%s':\n", keyword);
  int found = 0;

  for (int i = 0; i < nmovies; i++) {
    if (KMP(movies[i].title, keyword)) {
      printf("Titulo: %s, Genero: %s, Rating: %.1f, Duracion: %d\n",
             movies[i].title, movies[i].genre, movies[i].rating,
             movies[i].time);
      found = 1;
    }
  }

  if (!found) {
    printf("No se encontraron coincidencias para '%s'.\n", keyword);
  }
}
//-------------------------------------------------------------------------------
//--------------Cosas para el historial------------------------------------------
//usando queues para la complejidad
typedef struct QueueNode {
  Movie *movie;
  struct QueueNode *sgt;
} QueueNode;

typedef struct {
  QueueNode *inicio;
  QueueNode *final;
} Queue;

void initQueue(Queue *queue) {
  queue->inicio = NULL;
  queue->final = NULL;
}

//agregar al historial
void enqueue(Queue *queue, Movie *movie) {
  QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
  if (newNode == NULL) {
    printf("Error al asignar memoria para la cola.\n");
    return;
  }
  newNode->movie = movie;
  newNode->sgt = NULL;

  if (queue->final == NULL) {
    queue->inicio = queue->final = newNode;
  } else {
    queue->final->sgt = newNode;
    queue->final = newNode;
  }
  printf("Se ha agregadp '%s' al historial de reproduccion.\n", movie->title);
}

//eliminar la primer pelicula vista del historial
Movie *dequeue(Queue *queue) {
  if (queue->inicio == NULL) {
    printf("El historial esta vacio.\n");
    return NULL;
  }

  QueueNode *temp = queue->inicio;
  Movie *movie = temp->movie;
  queue->inicio = queue->inicio->sgt;

  if (queue->inicio == NULL) {
    queue->final = NULL;
  }

  free(temp);
  return movie;
}

//mostrar el historial
void mostrarhistorial(Queue *queue) {
  if (queue->inicio == NULL) {
    printf("El historial esta vacio.\n");
    return;
  }

  QueueNode *current = queue->inicio;
  printf("Historial de peliculas vistas:\n");
  while (current != NULL) {
    printf("Titulo: %s, Genero: %s, Rating: %.1f, Duracion: %d\n",
           current->movie->title, current->movie->genre, current->movie->rating,
           current->movie->time);
    current = current->sgt;
  }
}

//-------------------------------------------------------------------------------
// ----------------------------TOP 10--------------------------------------------
// ----------------------------HEAPSORT------------------------------------------
void heap(Movie arr[],int n, int  i) {
  int large = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && arr[left].rating < arr[large].rating)
    large = left;

  if (right < n && arr[right].rating < arr[large].rating)
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
//
void countingsortrating(Movie movies[], int num_movies) {
    // Ponemos el rango de rating que tenemos en el documento
    float min_rating = movies[0].rating;
    float max_rating = movies[0].rating;
    for (int i = 1; i < num_movies; i++) {
        if (movies[i].rating < min_rating) min_rating = movies[i].rating;
        if (movies[i].rating > max_rating) max_rating = movies[i].rating;
    }

    // Redondeamos las cantidades
    int min = (int)floor(min_rating);
    int max = (int)ceil(max_rating);
    int range = max - min + 1;

    // Creamos el array
    int *count = (int *)calloc(range, sizeof(int));
    if (count == NULL) {
        printf("Error al asignar memoria\n");
        return;
    }

    // Guardamos el conteo de cada ratign
    for (int i = 0; i < num_movies; i++) {
        int index = (int)round(movies[i].rating) - min;
        if (index >= 0 && index < range) {
            count[index]++;
        }
    }

    // Cambiar el conteo -> count[i] para que se igual a la posicion actual
    for (int i = 1; i < range; i++) {
        count[i] += count[i-1];
    }

    // Creamos el array dfonde se guardan los resultados ordenads
    Movie *output = (Movie *)malloc(num_movies * sizeof(Movie));
    if (output == NULL) {
        printf("Error al asignar memoria\n");
        free(count);
        return;
    }

    // Ordenamos los elementos
    for (int i = num_movies - 1; i >= 0; i--) {
        int index = (int)round(movies[i].rating) - min;
        if (index >= 0 && index < range) {
            output[count[index] - 1] = movies[i];
            count[index]--;
        }
    }

    // Ponemos el arreglo de salida sobre el orginal
    for (int i = 0; i < num_movies; i++) {
        movies[i] = output[i];
    }

    // Imprimimos las pelioculas de mayor a menor
    printf("Peliculas ordenadas por calificacion (%.1f-%.1f estrellas):\n", max_rating, min_rating);
    for (int i = num_movies -1; i >= 0; i--) {
        printf("Titulo: %s, Rating: %.1f, Duracion: %d\n",
               movies[i].title, movies[i].rating, movies[i].time);
    }

    // Liberamos memoria
    free(count);
    free(output);
}
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

// --------------- Metodo para menu --------------------------------------------
void menu(Movie *movies, int nmovies, Queue *movieQueue) {
  int x;

  printf(MAGENTA"\n+--------------------------------------------------------------+\n");
  printf("|                       Bienvenido a UVetflix                  |\n");
  printf("+--------------------------------------------------------------+"RESET"\n");

  do {
    printf(MAGENTA"+--------------------------------------------------------------+\n");
    printf("|                          MENU PRINCIPAL                      |\n");
    printf("+--------------------------------------------------------------+\n");
    printf("| 1. Mostrar peliculas agrupadas por genero                    |\n");
    printf("| 2. Mostrar top 10                                            |\n");
    printf("| 3. Peliculas ordenadas por calificacion (1-5 estrellas)      |\n");
    printf("| 4. Calcular cuantas peliculas puede ver en el tiempo deseado |\n");
    printf("| 5. Buscar pelicula                                           |\n");
    printf("| 6. Ver y eliminar primer pelicula en el historial            |\n");
    printf("| 7. Ver todo el historial de peliculas vistas                 |\n");
    printf("| 8. Salir                                                     |\n");
    printf("+--------------------------------------------------------------+"RESET"\n");
    printf("Elija la opcion deseada: ");
    scanf("%d", &x);
    getchar();

    switch (x) {
      case 1:
        group_by_genre(movies, nmovies);
        break;
      case 2:
        showtop10(movies, nmovies);
        break;
      case 3:
        countingsortrating(movies, nmovies);
        break;
      case 4:
        //ToDo: Calcular peliculas en t
        break;
      case 5: {
        char keyword[MAX_TITLE];
        printf("Introduce una palabra clave: ");
        fgets(keyword, sizeof(keyword), stdin);
        strtok(keyword, "\n"); //eliminar salto de línea

        int matchedIndices[nmovies];
        int matchCount = 0;

        printf("Resultados para '%s':\n", keyword);
        for (int i = 0; i < nmovies; i++) {
          if (KMP(movies[i].title, keyword)) {
            printf("%d. Titulo: %s, Genero: %s, Rating: %.1f, Duracion: %d\n",
                   matchCount + 1, movies[i].title, movies[i].genre, movies[i].rating,
                   movies[i].time);
            matchedIndices[matchCount++] = i; //guardar indice de coincidencia
          }
        }

        if (matchCount == 0) {
          printf("No se encontraron coincidencias para '%s'.\n", keyword);
        } else {
          char response;
          printf("Desea reproducir una? (s/n): ");
          scanf(" %c", &response);
          getchar();

          if (response == 's' || response == 'S') {
            int choice;
            printf("Seleccione el numero de la pelicula que desea ver: ");
            scanf("%d", &choice);
            getchar();

            if (choice > 0 && choice <= matchCount) {
              enqueue(movieQueue, &movies[matchedIndices[choice - 1]]);
            } else {
              printf("Seleccion invalida.\n");
            }
          }
        }
        break;
      }
      case 6: {
        Movie *firstMovie = dequeue(movieQueue);
        if (firstMovie != NULL) {
          printf("La primera pelicula en el historial es: %s\n", firstMovie->title);
          printf("Se ha eliminado '%s' del historial de reproduccion.\n", firstMovie->title);
        } else {
          printf("El historial está vacío.\n");
        }
        break;
      }
      case 7:
        mostrarhistorial(movieQueue);
        break;
      case 8:
        printf("Saliendo del programa.\n");
        break;
      default:
        printf("Opcion no valida.\n");
    }
  } while (x != 8);
}


// ------------------------------------------------------------------------------


int main() {
  int nmovies;
  Queue movieQueue;
  initQueue(&movieQueue);


  Movie *movies = input_movie("peliculas.txt", &nmovies);
  if (movies == NULL) {
    printf("Error reading movies\n");
    return 1;
  }

  menu(movies, nmovies, &movieQueue);

  free(movies);

  return 0;
}