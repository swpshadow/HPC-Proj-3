
/*
 * run "./generate_text <number of files>" in the same file location and have the input value for
 * number of files be at least half the number of threads to be used.
 *
 * to compile: gcc prod_cons.c -o prod_cons -fopenmp
 *
 * to run: ./prod_cons <number of threads>
*/

#include<stdio.h>
#include<stdlib.h>
#include <omp.h>
#include <string.h>

int MAX_LINE = 127;

void Tokenize(char*  lines[], int line_count, int thread_count);

int main(int argc, char const *argv[]) {


  int num_threads = strtol(argv[1], NULL, 10);
  int num_prod = num_threads/2;
  int num_cons = num_threads - num_prod;
  if (num_threads ==1)
  {
    num_cons=1;
    num_prod = 1;
  }
  int line_count = 0;
  char* lines[1000];
  FILE *f;
  char *file_name = malloc(16 * sizeof(char));
  int i = 0;
  double startomp = omp_get_wtime();
  #pragma omp parallel num_threads(num_prod)
  {
      int my_rank = omp_get_thread_num();
      sprintf(file_name, "./file%d", my_rank);
      char *buff = malloc(MAX_LINE*sizeof(char));
      char *l;
      f = fopen (file_name, "r");
      l = fgets(buff, 127, (FILE*)f);
      while(l != NULL){
        #pragma omp critical(addlines)
        {
          lines[i] = buff;
          i++;
          line_count++;
        }
        l = fgets(buff, 127, (FILE*)f);
        buff = malloc(MAX_LINE*sizeof(char));
      }
  }


  Tokenize(lines, line_count, num_cons);
  printf("total time:%f\n", omp_get_wtime() - startomp );
  return 0;
}

void Tokenize(char*  lines[], int line_count, int thread_count) {
   int my_rank, i, j;
   char *my_token, *saveptr;

#  pragma omp parallel num_threads(thread_count) \
      default(none) private(my_rank, i, j, my_token, saveptr) \
      shared(lines, line_count)
   {
      my_rank = omp_get_thread_num();
#     pragma omp for schedule(static, 1)
      for (i = 0; i < line_count; i++) {
         printf("Thread %d > line %d = %s", my_rank, i, lines[i]);
         j = 0;
         my_token = strtok_r(lines[i], " \t\n", &saveptr);
         while ( my_token != NULL ) {
            printf("Thread %d > token %d = %s\n", my_rank, j, my_token);
            my_token = strtok_r(NULL, " \t\n", &saveptr);
            j++;
         }
      if (lines[i] != NULL)
         printf("Thread %d > After tokenizing, my line = %s\n",
            my_rank, lines[i]);
      } /* for i */
   }  /* omp parallel */

}  /* Tokenize */
