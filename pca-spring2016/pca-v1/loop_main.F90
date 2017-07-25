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
! loop_tile uses rows, columns, threads, i_tile and j_tiles as input and gives
! time as output

program loop_main

   use loop_module

   implicit none

   integer :: M,N
   integer :: i_tile, j_tile
   real :: time
   integer :: threads
   real :: sumC, sumC2, sumC3, sumC4, sumC5

   M = 10000
   N = 10000

   threads = 1

   ! NAIVE IMPLEMENTATION
   ! Function call: call loop_naive(rows(in), columns(in), threads(in), time(out) )
   call loop_naive(M,N,threads,time,sumC)
   print '("Naive Loop Time = ",f10.7," seconds, and SUM = ",f32.7,"")',time,sumC
   ! END OF NAIVE IMPLEMENTATION

   ! TILE IMPLEMENTATION
   i_tile = 1
   j_tile = 20
   ! Function call loop_tile(rows(in), columns(in), threads(in), i_tile(in), j_tile (in) time(out))
   call loop_tile(M,N,threads,i_tile,j_tile,time,sumC2)
   print '("Tiled Loop Time = ",f10.7," seconds, and SUM = ",f32.7,"")',time,sumC2
   ! END OF TILE IMPLEMENTATION


   ! Adding more calls for different tile sizes
   
   i_tile = 1
   j_tile = 50
   call loop_tile(M,N,threads,i_tile,j_tile,time,sumC3)
   print '("Tiled Loop Time = ",f10.7," seconds, and SUM = ",f32.7,"")',time,sumC3

   i_tile = 1
   j_tile = 100
   call loop_tile(M,N,threads,i_tile,j_tile,time,sumC4)
   print '("Tiled Loop Time = ",f10.7," seconds, and SUM = ",f32.7,"")',time,sumC4

   i_tile = 1
   j_tile = 200
   call loop_tile(M,N,threads,i_tile,j_tile,time,sumC5)
   print '("Tiled Loop Time = ",f10.7," seconds, and SUM = ",f32.7,"")',time,sumC5


end program loop_main
