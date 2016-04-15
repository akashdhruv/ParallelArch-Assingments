subroutine loop_naive(M,N,th,time)

   !$ use omp_lib

   implicit none
   integer, intent(in) :: M,N,th
   real, intent(out) :: time

   double precision, dimension(:,:), allocatable :: A
   double precision, dimension(:), allocatable :: b, c
   integer :: i,j
   double precision :: start,finish

   allocate(A(N,M))
   allocate(b(M))
   allocate(c(N))

   ! In Fortran arrays are column major

   !$OMP PARALLEL DO PRIVATE(i,j) SHARED(M,N,A) NUM_THREADS(th)
   do j=1,M
    do i=1,N
       A(i,j) = i*M+j
    end do
   end do
   !$OMP END PARALLEL DO

   !$OMP PARALLEL DO PRIVATE(j) SHARED(b,M) NUM_THREADS(th)
   do j=1,M
     b(j) = j**2
   end do
   !$OMP END PARALLEL DO

   start = omp_get_wtime()
   !$OMP PARALLEL DO PRIVATE(i,j) SHARED(M,N,A,b,c) NUM_THREADS(th)
   do j=1,M
    do i=1,N
       c(i) = c(i) + A(i,j) * b(j)
    end do
   end do
   !$OMP END PARALLEL DO
   finish = omp_get_wtime()

   time = finish-start

   deallocate(A)
   deallocate(b)
   deallocate(c)

end subroutine loop_naive


subroutine loop_tile(M,N,th,i_tile,j_tile,time)

   !$ use omp_lib
   implicit none
   integer, intent(in) :: M,N,th
   integer, intent(in) :: i_tile,j_tile
   real, intent(out) :: time

   double precision, dimension(:,:), allocatable :: A
   double precision, dimension(:), allocatable :: b, c
   integer :: i,j,ii,jj
   double precision :: start,finish

   allocate(A(N,M))
   allocate(b(M))
   allocate(c(N))

   ! In Fortran arrays are column major

   !$OMP PARALLEL DO PRIVATE(i,j) SHARED(M,N,A) NUM_THREADS(th)
   do j=1,M
    do i=1,N
       A(i,j) = i*M+j
    end do
   end do
   !$OMP END PARALLEL DO

   !$OMP PARALLEL DO PRIVATE(j) SHARED(b,M) NUM_THREADS(th)
   do j=1,M
     b(j) = j**2
   end do
   !$OMP END PARALLEL DO

   start = omp_get_wtime()
   !$OMP PARALLEL DO PRIVATE(jj,ii,j,i) SHARED(A,b,c,N,M,i_tile,j_tile)  NUM_THREADS(th)
   do jj=1,M,j_tile
    do ii=1,N,i_tile
     do j=jj,jj+j_tile-1 ! Fortran indices go from 1 to N and not 0 to N-1
      do i=ii,ii+i_tile-1
        c(i) = c(i) + A(i,j) * b(j)
      end do
     end do
    end do
   end do
   !$OMP END PARALLEL DO

   finish = omp_get_wtime()

   time = finish-start

   deallocate(A)
   deallocate(b)
   deallocate(c)

end subroutine loop_tile

