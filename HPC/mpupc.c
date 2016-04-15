#include<stdio.h>
#include<upc_relaxed.h>
#include<mpi.h>

shared int rank;
shared int size;

int main(int argc, char *argv[])

{
	//int rank,size;

	if(MYTHREAD==0)
	{
		MPI_Init(&argc, &argv);
        	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        	MPI_Comm_size(MPI_COMM_WORLD, &size);
	}

	upc_barrier;

	printf("\nHello from Thread %d of %d on Node %d, rank %d\n",MYTHREAD,THREADS,rank+1,rank);

	upc_barrier;

	if(MYTHREAD==0)
		MPI_Finalize();

	return 0;
}
