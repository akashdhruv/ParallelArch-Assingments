module loop_module

   implicit none
    
     interface

       subroutine loop_naive(M,N,th,time)
       implicit none
       integer, intent(in) :: M,N,th
       real, intent(out) :: time
       end subroutine loop_naive

       subroutine loop_tile(M,N,th,i_tile,j_tile,time)
       implicit none
       integer, intent(in) :: M,N,th,i_tile,j_tile
       real, intent(out) :: time
       end subroutine loop_tile

     end interface

end module loop_module
