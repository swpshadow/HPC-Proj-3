
/*
 * Generates text for the producer consumer problem.
 * at a minimum you should generate half the number of threads you plan to use
 *
 * to compile: gcc generate_text.c -o generate_text
 * to run: ./generate_text <number of files>
*/



#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[]) {

  FILE *fp;
  int n = strtol(argv[1], NULL, 10);
  char *file_name = malloc(10 * sizeof(char));
  char *word = malloc(12 * sizeof(char));
  for (int i = 0; i < n; i ++)
  {
    sprintf(file_name, "file%d", i);
    fp = fopen (file_name, "w");
    for (int j = 0; j < (rand() +1) % 10 + 10; j++)
    {
      sprintf(word, "word%da%d ", i, j);
      fputs(word, fp);
    }
    fputs("\n", fp);
    int j;
    for (j = 0; j < (rand() +1) % 10 + 10; j++)
    {
      sprintf(word, "word%d%da%d ", 2, i, j);
      fputs(word, fp);
    }
    sprintf(word, "word%d%da%d", 2, i, j);
    fputs(word, fp);
  }
  return 0;
}
