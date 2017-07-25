#include <upc.h>
#include <mpi.h>

#define N THREADS

shared double v1[N], v2[N];
shared int r0;

int main(int argc, char **argv) {

	int i, me, np;
	double sum = 0.0, dotp;
	MPI_Comm hybrid_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if (MYTHREAD == 0) r0 = me;
	upc_barrier;

	MPI_Comm_split(MPI_COMM_WORLD, 0, r0*THREADS+MYTHREAD, &hybrid_comm);
	MPI_Comm_rank(hybrid_comm, &me);
	MPI_Comm_size(hybrid_comm, &np);
	upc_barrier;

	//printf("Hello from Thread %d of %d and rank %d of %d\n",MYTHREAD,THREAD,me,np);
	printf("Hello from rank %d of %d\n",me,np);
	MPI_Finalize();

	return 0;
}
