#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

double get_wall_time()
{
        struct timeval time;
        gettimeofday(&time, NULL);
        return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])

{

   int i,j,ii,jj,i_tile,j_tile;

   const int N=100000;
   double sumC_n, sumC_t;

   double *A = malloc(sizeof(double)*N*N); 
   double *b = malloc(sizeof(double)*N);
   double *c_naive = malloc(sizeof(double)*N);
   double *c_tile=malloc(sizeof(double)*N);

   sumC_n = 0.0;
   sumC_t = 0.0;

   j_tile = 10000;
   i_tile = 10000;

   for (i=0; i<N; i++){
     for (j=0; j<N; j++){
        A[i*N+j] = (i+1)*N+j+1;
     }
   }

   for (i=0; i<N; i++){
       b[i]       = (i+1)^2;
       c_naive[i] = 0.0;
       c_tile[i]  = 0.0;
   }


   double start_n=get_wall_time();
   for(i=0; i<N; i++){
     for(j=0; j<N; j++){
        c_naive[i] += A[i*N+j]*b[j];
     }
   }   
   double finish_n=get_wall_time();

   double start=get_wall_time();  
   //#pragma omp for private(i,j,ii,jj) shared(A,b,c,i_tile,j_tile) num_threads(4)
     for(jj=0; jj<N; jj+=j_tile){
       for(i=0; i<N; i++){
        for(j=jj; j<jj+j_tile; j++){
               c_tile[i] += A[i*N+j]*b[j];
        }
      }
     }
   double finish=get_wall_time();

  for(i=0;i<N;i++){
 
     sumC_n = sumC_n + c_naive[i];
     sumC_t = sumC_t + c_tile[i];

  }

  free(A);
  free(b);
  free(c_naive);
  free(c_tile);

  printf("Naive Time: %f seconds and SUM: %f\n",finish_n-start_n,sumC_n);
  printf("Tiled Time: %f seconds and SUM: %f\n",finish-start,sumC_t);
  
  return 0;

}
