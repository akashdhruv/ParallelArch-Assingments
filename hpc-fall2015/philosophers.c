#include <stdlib.h>
#include <stdio.h>
#include <upc.h>
#define N 1 // Number of lunches

typedef int bool;
#define true 1
#define false 0

shared bool permission[THREADS];
shared bool ate_this_round[THREADS];
upc_lock_t *shared forks[THREADS];

int main(){

//upc_lock_t *forks[THREADS];

for(int i=0; i<THREADS; i++)
	forks[i] =upc_all_lock_alloc();

upc_barrier;

for(int j=0; j<N; j++)
	{
		//if(MYTHREAD%2==1)
			permission[MYTHREAD] = false;
		//else
		//	permission[MYTHREAD] = true;

		ate_this_round[MYTHREAD] = false;

		//printf("Eating Status of philsoper #%d = %d",MYTHREAD,ate_this_round[MYTHREAD]);

		//upc_barrier;

		while(ate_this_round[MYTHREAD]==false)
		{

		if(permission[(MYTHREAD+1)%THREADS]==false && permission[(MYTHREAD-1+THREADS)%THREADS]==false)
			{
			permission[MYTHREAD] = true;
			upc_lock(forks[MYTHREAD]);
			upc_lock(forks[(MYTHREAD+1)%THREADS]);
			printf("philosopher # %d got the right and left forks\n",MYTHREAD); 
			//sleep(5); //eat
			upc_unlock(forks[(MYTHREAD+1)%THREADS]);
			upc_unlock(forks[MYTHREAD]);
			ate_this_round[MYTHREAD] = true;
			permission[MYTHREAD] = false;
			}
		else
			{continue;}

      		}

      		upc_barrier;

		//if(MYTHREAD==0)
		//	printf("\n -------------Round Complete------------ \n");

		//upc_barrier;
      }

      if (MYTHREAD==0)
      for(int i=0; i<THREADS; i++)
	      upc_lock_free(forks[i]);

      return 0; }

