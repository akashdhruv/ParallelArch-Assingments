program loop_main

   use loop_module

   implicit none

   integer :: M,N
   integer :: i_tile, j_tile
   real :: time
   integer :: threads

   M = 2**13
   N = 2**13

   threads = 2**1

   ! Function call loop_naive(rows(in), columns(in), threads(in), time(out) )
   call loop_naive(M,N,threads,time)

   print '("Naive Loop Time = ",f10.7," seconds.")',time

   i_tile = 2**2
   j_tile = 2**2
   
   ! Function call loop_tile(rows(in), columns(in), threads(in), i_tile(in), j_tile (in) time(out))
   call loop_tile(M,N,threads,i_tile,j_tile,time)


   print '("Tiled Loop Time = ",f10.7," seconds.")',time

   i_tile = 2**3
   j_tile = 2**3

   ! Function call loop_tile(rows(in), columns(in), threads(in), i_tile(in), j_tile (in) time(out))
   call loop_tile(M,N,threads,i_tile,j_tile,time)


   print '("Tiled Loop Time = ",f10.7," seconds.")',time


end program loop_main
