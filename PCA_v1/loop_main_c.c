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

   const int N=2^20;

   double *A = malloc(sizeof(double)*N*N); 
   double *b = malloc(sizeof(double)*N);
   double *c = malloc(sizeof(double)*N);

   i_tile = 8;
   j_tile = 8;

   for (i=0; i<N; i++){
     for (j=0; j<N; j++){
        A[i*N+j] = i*N+j;
     }
   }

   for (i=0; i<N; i++){
       b[i] = i^2;
   }

   double start=get_wall_time();  
   #pragma omp for private(i,j,ii,jj) shared(A,b,c,i_tile,j_tile) num_threads(4)
   for(ii=0; ii<N; ii+=i_tile){
     for(jj=0; jj<N; jj+=j_tile){
       for(i=ii; i<ii+i_tile; i++){
        for(j=jj; j<jj+j_tile; j++){
               c[i] += A[i*N+j]*b[j];
        }
      }
     }
   }
   double finish=get_wall_time();

  free(A);
  free(b);
  free(c);

  printf("Time: %f seconds\n",finish-start);
  
  return 0;

}
