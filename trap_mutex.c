
/*
 *note: n must be divisible by the number of threads
 *to compile: gcc trap_mutex.c -o trap_mutex -fopenmp
 *to run ./trap_mutex <number of threads>
 *
*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include <omp.h>


int num_threads,n = 0;
double sum = 0;
pthread_mutex_t lock;

struct arg_struct{
  double a;
  double b;
  int thread;
};

void* trap(void *args);

double f(double x);

int main(int argc, char* argv[]){
  clock_t start, end;
  double a, b;
  num_threads = strtol(argv[1], NULL, 10);
  printf("Enter a, b, and n\n");
  scanf("%lf %lf %d", &a, &b, &n);
  if(n/num_threads * num_threads != n)
  {
    printf("%s\n", "n must be divisible by the number of threads");
    return 1;
  }
  if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
  start = clock();
  double startomp = omp_get_wtime();
  pthread_t* thread_handles;
  thread_handles = malloc(num_threads*sizeof(pthread_t));

  struct arg_struct *args;
  args = malloc(num_threads * sizeof(struct arg_struct));
  for (int thread = 0; thread < num_threads; thread++)
  {
    args[thread].a = a;
    args[thread].b = b;
    args[thread].thread = thread;
    pthread_create(&thread_handles[thread], NULL, trap, (void*) &args[thread]);
  }

  for(int thread = 0; thread < num_threads; thread++)
  {
    pthread_join(thread_handles[thread], NULL);
  }
  end = clock();
  printf("total time:%f\n", omp_get_wtime() - startomp );

  printf("total clock ticks: %ld and time: %lf\n", end - start, (double)(end-start)/CLOCKS_PER_SEC );
  printf("%s %f\n", "the result is: ", sum );
  free(thread_handles);
  return 0;
}

void* trap(void *arguments)
{
  struct arg_struct *args = arguments;
  double a = args->a;
  double b = args->b;
  int my_rank = args->thread;
   double h = (b-a)/n;
   double x = 0;
   double my_result = 0;
   int local_n = (int)(n/num_threads);
   double local_a = a + my_rank*local_n*h;
   double local_b = local_a + local_n*h;
   my_result = (f(local_a) + f(local_b))/2.0;
   for (int i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h;
   pthread_mutex_lock(&lock);
   sum += my_result;
   pthread_mutex_unlock(&lock);
   return NULL;
}

double f( double x){
  return x * x;
}
