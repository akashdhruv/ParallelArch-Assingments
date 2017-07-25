#include <upc.h>
#include <mpi.h>
#include <stdio.h>

#define N 10000000*THREADS

shared double v1[N], v2[N];
shared double our_sum = 0.0;
shared double my_sum[THREADS];
shared int me, np;

double get_wall_time()
{
        struct timeval time;
        gettimeofday(&time, NULL);
        return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char **argv) {

	float start=get_wall_time();
	int i, B;
	double dotp;
	if (MYTHREAD == 0) {
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD,(int*)&me);
		MPI_Comm_size(MPI_COMM_WORLD,(int*)&np);
	}

	if(MYTHREAD==0){
		for(i=me; i<N; i=i+np) {
			v1[i]=i;
			v2[i]=i+1;
		}
	}

	upc_barrier;

	if(MYTHREAD==0)
		MPI_Barrier(MPI_COMM_WORLD);

	B = N/np;
	my_sum[MYTHREAD] = 0.0;

	upc_forall(i=me;i<N;i=i+np; &v1[i])
		my_sum[MYTHREAD] += v1[i]*v2[i];
	upc_barrier;

	upc_forall(i=0;i<THREADS;i++; &my_sum)
		our_sum=our_sum+my_sum[i];
	upc_barrier;

	if (MYTHREAD == 0) {
		MPI_Reduce(&our_sum, &dotp, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if (me == 0)
			printf("Dot = %f\n", dotp); 
		MPI_Finalize();
	}

	float end=get_wall_time();
	printf("Elapsed Time: %f\n",end-start);
	return 0;
	}
