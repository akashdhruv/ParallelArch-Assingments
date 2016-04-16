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
   real :: time(5,10)
   real :: avgtime(5)
   integer :: threads
   integer :: i,j

   time = 0.0
   avgtime = 0.0

   M = 2**13
   N = 2**13

   threads = 2**2

   do j = 1,10 ! Repeating procedure 10 times to get average time

   ! NAIVE IMPLEMENTATION
   ! Function call: call loop_naive(rows(in), columns(in), threads(in), time(out) )
   call loop_naive(M,N,threads,time(1,j))
   !print '("Naive Loop Time = ",f10.7," seconds.")',time
   ! END OF NAIVE IMPLEMENTATION

   ! TILE IMPLEMENTATION
   i_tile = 2**2
   j_tile = 2**2
   ! Function call loop_tile(rows(in), columns(in), threads(in), i_tile(in), j_tile (in) time(out))
   call loop_tile(M,N,threads,i_tile,j_tile,time(2,j))
   !print '("Tiled Loop Time = ",f10.7," seconds.")',time
   ! END OF TILE IMPLEMENTATION


   ! Adding more calls for different tile sizes
   
   i_tile = 2**4
   j_tile = 2**4
   call loop_tile(M,N,threads,i_tile,j_tile,time(3,j))
   !print '("Tiled Loop Time = ",f10.7," seconds.")',time

   i_tile = 2**8
   j_tile = 2**8
   call loop_tile(M,N,threads,i_tile,j_tile,time(4,j))
   !print '("Tiled Loop Time = ",f10.7," seconds.")',time

   i_tile = 2**10
   j_tile = 2**10
   call loop_tile(M,N,threads,i_tile,j_tile,time(5,j))
   !print '("Tiled Loop Time = ",f10.7," seconds.")',time

   end do 

   ! Calculating average time
   do i = 1,5
     do j = 1,10    
         avgtime(i) = avgtime(i) + time(i,j) 
     end do
   end do
   avgtime = avgtime/10

   ! Displaying time
   do i = 1,5
      print '("Time = ",f10.7," seconds.")',avgtime(i)
   end do

end program loop_main
