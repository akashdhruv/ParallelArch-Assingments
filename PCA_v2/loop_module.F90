! This is like a  header file which declares subroutines and defines their
! argument list.

! intent(in) means that the variable enters the subroutine with a value
! intent(out) means that the variable leaves the subroutine with a value
! intent(inout) means that the variable enters the subroutine with a values and
! leaves with a value.

! the variable passed in the subroutine call in the main program will have the
! value depending on the intent of the corresponding dummy variable.

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
