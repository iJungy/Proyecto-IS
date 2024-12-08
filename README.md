# Proyecto-IS
Sistema de Gestion de Streaming
Instrucciones


Objetivo: Desarrollar un programa que simule algunas funcionalidades clave de una aplicación de gestión de biblioteca de películas, similar a plataformas como Netflix o Amazon Prime. 

# Funcionalidades y Requerimientos de Complejidad: 

Exploración de Películas: 
Agrupación de películas por género. Complejidad: O(nlogn). 
Top 10 películas más populares por calificación. Complejidad: O(nlogn). 
Películas ordenadas por calificación (1-5 estrellas), de mayor a menor. Complejidad: MENOR a O(nlogn). 
Cantidad de películas que se pueden ver para acumular un tiempo exacto de 't' (similar al problema de la mochila) e indicar cuales son. Complejidad menor a O(n^3). Si no da un tiempo exacto, se regresa que no se puede 
Búsqueda de Películas: 
Búsqueda por título o palabras clave. Complejidad: MENOR a O(n^2)  
Historial de películas: 
Cada vez que se ve una película, se mantiene un historial de visualización. Este registro debe ser O(1) 
Se puede visualizar siempre la primera película que se vió del historial y eliminarla con complejidad O(1) 
Se puede visualizar todo el historial de películas O(n) 
El documento deberá incluir: 

# Descripción de los algoritmos utilizados para cada funcionalidad. 
Justificación de la elección de cada algoritmo. 
Explicación del funcionamiento de cada algoritmo y su clasificación (por ejemplo, divide y vencerás, etc.). 
Cálculo de la eficiencia para el peor, mejor y caso promedio. 
Cálculo de la complejidad para el peor, mejor y caso promedio (O, Ω, θ). 

# Aspectos Obligatorios: 
Utilizar algoritmos eficientes y adecuados para cada funcionalidad. 
Respetar las restricciones de complejidad establecidas. 
No repetir el mismo algoritmo para diferentes funcionalidades, incluso si tienen la misma complejidad. 
La información se debe cargar desde un archivo. Son libres de generar tantos archivos como necesiten (conceptos de base de datos para el enlace de la información) 
