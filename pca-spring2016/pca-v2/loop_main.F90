! This is the main program which calls other subroutines to implement matrix
! operations. The subroutines are defined in the file loop_subroutines.F90 and
! are declared in the module loop_module.F90. Modules in fortran are like header
! files. 

! The ** operator is the power operator in fortran. Therefore, 2**3 = 8

! loop_naive and loop_tile are the subroutines defined in the file
! loop_subroutines.F90.

! The subroutines are called with the keyword call
! call loop_naive(rows, columns, threads, time)
! call loop_tile(rows, columns, threads, i_tile, j_tile, time)

! loop_naive uses rows, columns and threads as input and gives time as output
! loop_tile uses rows, columns, threads, i_tile and j_tile as input and gives
! time as output

program loop_main

   use loop_module

   implicit none

   integer :: M,N
   integer :: i_tile, j_tile

   real :: time(6,10)
   real :: avgtime(5)

   double precision :: sumc(6,10)
   double precision :: avgsum(6)

   integer :: threads
   integer :: i,j
   
   time = 0.0
   avgtime = 0.0
   sumc = 0.0
   avgsum = 0.0

   M = 2**10
   N = 2**10

   threads = 2**2

   do j = 1,10 ! Repeating procedure 10 times to get average time

   ! NAIVE IMPLEMENTATION
   ! Function call: call loop_naive(rows(in), columns(in), threads(in), time(out) )
   call loop_naive(M,N,threads,time(1,j),sumc(1,j))
   ! END OF NAIVE IMPLEMENTATION

   ! TILE IMPLEMENTATION
   i_tile = 2**1
   j_tile = 2**1
   ! Function call loop_tile(rows(in), columns(in), threads(in), i_tile(in), j_tile (in) time(out))
   call loop_tile(M,N,threads,i_tile,j_tile,time(2,j),sumc(2,j))
   ! END OF TILE IMPLEMENTATION


   ! Adding more calls for different tile sizes
   
   i_tile = 2**1
   j_tile = 2**4
   !call loop_tile(M,N,threads,i_tile,j_tile,time(3,j),sumc(3,j))

   i_tile = 2**4
   j_tile = 2**1
   !call loop_tile(M,N,threads,i_tile,j_tile,time(4,j),sumc(4,j))

   i_tile = 2**8
   j_tile = 2**1
   !call loop_tile(M,N,threads,i_tile,j_tile,time(5,j),sumc(5,j))

   i_tile = 2**1
   j_tile = 2**8
   !call loop_tile(M,N,threads,i_tile,j_tile,time(6,j),sumc(6,j))

   end do 

   ! Calculating average time and sum
   do i = 1,6
     do j = 1,10    
         avgtime(i) = avgtime(i) + time(i,j) 
         avgsum(i)  = avgsum(i)  + sumc(i,j)
     end do
   end do
   avgtime = avgtime/10
   avgsum  = avgsum/10

   ! Displaying time
   do i = 1,6
      print '("Time = ",f10.7," seconds & Checksum = ",e10.3,"")',avgtime(i),avgsum(i)
   end do

end program loop_main
