FF = gfortran
CC = cc

FFLAGS = -fopenmp
CFLAGS = -fopenmp

# Fortran 
loop_f: loop_main.o loop_subroutines.o
	$(FF) $(FFLAGS) -o loop_f loop_main.o loop_subroutines.o

loop_subroutines.o: loop_subroutines.F90
	$(FF) $(FFLAGS) -c loop_subroutines.F90

loop_module.o loop_module.mod: loop_module.F90
	$(FF) $(FFLAGS) -c loop_module.F90

loop_main.o: loop_main.F90 loop_module.mod
	$(FF) $(FFLAGS) -c loop_main.F90

# C
loop_c: loop_main_c.o
	$(CC) $(CFLAGS) -o loop_c loop_main_c.o

loop_main_c.o: loop_main_c.c
	$(CC) $(CFLAGS) -c loop_main_c.c

all: loop_c loop_f

clean:
	rm *.o *.mod loop_f loop_c
