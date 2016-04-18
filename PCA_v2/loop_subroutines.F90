! This file includes subroutine definitions. Local variables declared in the
! definitions should not be declared in the module file. Module file only
! defines the argument list.

! use omp_lib is the OpenMP module which includes the function omp_get_wtime()


subroutine loop_naive(M,N,th,time,sumc)

   !$ use omp_lib

   ! Argument Variables
   implicit none
   integer, intent(in) :: M,N,th
   real, intent(out) :: time
   double precision, intent(out) :: sumc

   ! Local Variables
   double precision, dimension(:,:), allocatable :: A
   double precision, dimension(:), allocatable :: b, c, c_priv
   integer :: i,j
   double precision :: start,finish

   ! Memory Allocation
   allocate(A(N,M))
   allocate(b(M))
   allocate(c(N))
  
   A = 0.0
   b = 0.0
   c = 0.0
  
   ! In Fortran arrays are column major
   
   ! Populating Arrays

   do j=1,M
    do i=1,N
       A(i,j) = i*M+j
    end do
   end do

   do j=1,M
     b(j) = j**2
   end do
 
   start = omp_get_wtime()
 
   !---------- PARALLELIZATION -------------!
 
   !$OMP PARALLEL NUM_THREADS(th) SHARED(M,N,A,b,c) PRIVATE(c_priv,i,j)

   ! Allocating private array c_priv
   allocate(c_priv(N))
   c_priv = 0.0
  
   !$OMP DO COLLAPSE(2)
   do j=1,M
    do i=1,N
       c_priv(i) = c_priv(i) + A(i,j) * b(j)
    end do
   end do
   !$OMP END DO

   !$OMP CRITICAL
   do i=1,N
       c(i) = c(i) + c_priv(i)
   end do
   !$OMP END CRITICAL

   ! Deallocating private array c_priv
   deallocate(c_priv)
   
   !$OMP END PARALLEL

   !--------- END OF PARALLELIZATION ---------!

   finish = omp_get_wtime()
   time = finish - start

   ! Checksum
   do i =1,N
     sumc = sumc + c(i)
   end do
 
   ! Memory Deallocation
   deallocate(A)
   deallocate(b)
   deallocate(c)

end subroutine loop_naive


subroutine loop_tile(M,N,th,i_tile,j_tile,time,sumc)

   !$ use omp_lib

   ! Argument Variables
   implicit none
   integer, intent(in) :: M,N,th
   integer, intent(in) :: i_tile,j_tile
   real, intent(out) :: time
   double precision, intent(out) :: sumc

   ! Local Variables
   double precision, dimension(:,:), allocatable :: A
   double precision, dimension(:), allocatable :: b, c, c_priv
   integer :: i,j,ii,jj
   double precision :: start,finish

   ! Memory Allocation
   allocate(A(N,M))
   allocate(b(M))
   allocate(c(N))

   A = 0.0
   b = 0.0
   c = 0.0

   ! In Fortran arrays are column major

   ! Populating Arrays

   do j=1,M
    do i=1,N
       A(i,j) = i*M+j
    end do
   end do
   
   do j=1,M
     b(j) = j**2
   end do

   start = omp_get_wtime()

   !---------- PARALLELIZATION -------------!
  
   !$OMP PARALLEL PRIVATE(c_priv,jj,ii,j,i) SHARED(A,b,c,M,N,i_tile,j_tile) NUM_THREADS(th) 

   allocate(c_priv(N))
   c_priv = 0.0

   !$OMP DO COLLAPSE(2)
   do jj=1,M,j_tile
    do ii=1,N,i_tile
     do j=jj,jj+j_tile-1 ! Fortran indices go from 1 to N and not 0 to N-1
      do i=ii,ii+i_tile-1
        c_priv(i) = c_priv(i) + A(i,j) * b(j)
      end do
     end do
    end do
   end do
   !$OMP END DO

   !$OMP CRITICAL
   do i=1,N
       c(i) = c(i) + c_priv(i)
   end do
   !$OMP END CRITICAL

   deallocate(c_priv)

   !$OMP END PARALLEL

   !--------- END OF PARALLELIZATION ---------!

   finish = omp_get_wtime()
   time = finish-start

   ! Checksum
   do i =1,N
     sumc = sumc + c(i)
   end do
   
   ! Memory Deallocation
   deallocate(A)
   deallocate(b)
   deallocate(c)

end subroutine loop_tile

