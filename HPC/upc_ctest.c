#include<stdio.h>
#include<upc.h>

int main(int argc, char *argv[])

{

	printf("\nHello from Thread %d of %d\n",MYTHREAD,THREADS);

	return 0;
}
