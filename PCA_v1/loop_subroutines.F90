! This file includes subroutine definitions. Local variables declared in the
! definitions should not be declared in the module file. Module file only
! defines the argument list.

! use omp_lib is the OpenMP module which includes the function omp_get_wtime()


subroutine loop_naive(M,N,th,time,sumC)

   !$ use omp_lib

   ! Argument Variables
   implicit none
   integer, intent(in) :: M,N,th
   real, intent(out) :: time
   real, intent(out) :: sumC

   ! Local Variables
   double precision, dimension(:,:), allocatable :: A
   double precision, dimension(:), allocatable :: b, c, c_priv
   integer :: i,j
   double precision :: start,finish

   ! Memory Allocation
   allocate(A(N,M))
   allocate(b(M))
   allocate(c(N))
   allocate(c_priv(N))

   c    = 0.0
   time = 0.0

   ! In Fortran arrays are column major

   !$OMP PARALLEL PRIVATE(i,j,start,finish,c_priv) shared(M,N,A,b,c,time) NUM_THREADS(th)

   c_priv = 0.0

   !$OMP DO SCHEDULE(STATIC)
   do j=1,M
    do i=1,N
       A(i,j) = i*M+j
    end do
   end do
   !$OMP END DO

   !$OMP DO SCHEDULE(STATIC)
   do j=1,M
     b(j) = j**2
   end do
   !$OMP END DO

   start = omp_get_wtime()

   !$OMP DO SCHEDULE(STATIC)
   do j=1,M
    do i=1,N
       c(i) = c(i) + A(i,j) * b(j)
    end do
   end do
   !$OMP END DO

   finish    = omp_get_wtime()

   !!$OMP CRITICAL
   !do i=1,N
   !   c(i) = c(i) + c_priv(i)
   !end do
   !!$OMP END CRITICAL

   !$OMP ATOMIC
   time = time + (finish-start)

   !$OMP END PARALLEL

   time = time/th
   sumC = sum(c)

   ! Memory Deallocation
   deallocate(A)
   deallocate(b)
   deallocate(c,c_priv)

end subroutine loop_naive


subroutine loop_tile(M,N,th,i_tile,j_tile,time,sumC)

   !$ use omp_lib

   ! Argument Variables
   implicit none
   integer, intent(in) :: M,N,th
   integer, intent(in) :: i_tile,j_tile
   real, intent(out) :: time
   real, intent(out) :: sumC

   ! Local Variables
   double precision, dimension(:,:), allocatable :: A
   double precision, dimension(:), allocatable :: b, c, c_priv
   integer :: i,j,ii,jj
   double precision :: start,finish

   ! Memory Allocation
   allocate(A(N,M))
   allocate(b(M))
   allocate(c(N))
   allocate(c_priv(N))

   c    = 0.0
   time = 0.0

   ! In Fortran arrays are column major

   !$OMP PARALLEL PRIVATE(jj,ii,j,i,c_priv,start,finish) SHARED(A,b,c,N,M,i_tile,j_tile,time) NUM_THREADS(th)

   c_priv = 0.0

   !$OMP DO SCHEDULE(STATIC)
   do j=1,M
    do i=1,N
       A(i,j) = i*M+j
    end do
   end do
   !$OMP END DO

   !$OMP DO SCHEDULE(STATIC)
   do j=1,M
     b(j) = j**2
   end do
   !$OMP END DO

   start = omp_get_wtime()

   !$OMP DO SCHEDULE(STATIC)
   do jj=1,M,j_tile
     do j=jj,jj+j_tile-1 ! Fortran indices go from 1 to N and not 0 to N-1
      do i=1,N
        c(i) = c(i) + A(i,j) * b(j)
      end do
     end do
    end do
   !$OMP END DO

   finish = omp_get_wtime()

   !!$OMP CRITICAL
   !do i=1,N
   !   c(i) = c(i) + c_priv(i)
   !end do
   !!$OMP END CRITICAL

   !$OMP ATOMIC
   time = time + (finish-start)

   !$OMP END PARALLEL

   time = time/th
   sumC = sum(c)

   ! Memory Deallocation
   deallocate(A)
   deallocate(b)
   deallocate(c,c_priv)

end subroutine loop_tile

