module timing_mod
   !use container
   implicit none
   type times
      real :: t1, t2, tm
      real :: min, max
      integer :: count
      character(32) :: name = ""
   end type times

   type container
      type(times), dimension(100) :: times
      !type(times), dimension(:), allocatable :: times
      integer nb
   end type container

   type(container), save, target :: timers

contains
   subroutine start_timer(which)
      integer which
      call cpu_time(timers%times(which)%t1)
   end subroutine start_timer
   !------------------------------------
   subroutine stop_timer(which)
      integer which
      real dt
      type(times), pointer :: t
      t => timers%times(which)
      call cpu_time(t%t2)
      dt = t%t2 - t%t1
      t%tm = t%tm + dt
      t%count = t%count + 1
      t%max = max(t%max, dt)
      t%min = min(t%max, dt)
      !call cpu_time(timers%times(which)%t2)
      !timers%times(which)%tm = timers%times(which)%t2 - timers%times(which)%t1
   end subroutine stop_timer
   !------------------------------------
   subroutine setup_timers(nb)
   integer nb
   if (nb > 100) then
     print*,'cannot allocate more than 100 timers'
   end if
   !allocate(timers%times(nb))
   timers%nb = nb
   return
   end subroutine setup_timers
   !------------------------------------
   subroutine setup_timer(which, name)
   integer which
   character(*) :: name
   type(times), pointer :: t

   if (which > timers%nb) then
      stop 'exceeded maximum number of timers'
   end if

   t => timers%times(which)
   t%name = name
   t%t1 = 0.
   t%t2 = 0.
   t%min = 0.
   t%max = 0.
   t%count = 0

   !timers%times(which)%name = name
   !timers%times(which)%t1 = 0.
   !timers%times(which)%t2 = 0.
   return
   end subroutine setup_timer
   !------------------------------------
   subroutine dump_timings()
   real t, total, avg
   integer i, lg, count, mc
   type(times), pointer :: tm
   character(32), pointer :: n

   mc=10000
   print*
   print*,'LIST OF TIMINGS (ms)'
   print*,'---------------'
   print*,'     count         total       N         avg  '
   print*,'---------------'

   do i=1,timers%nb
      tm => timers%times(i)
      total = tm%tm*1e3
      avg = total / tm%count
      n => tm%name
      lg = len_trim(n)
      if (lg .ne. 0) then
         write(*,100) tm%count, total, mc, avg
      end if
      !write(*,100) timers%times(i)%name(1:len_trim(timers%times(i)%name)), t
   end do

100 format((5x,i5), (5x,f10.4), i10, (3x,f10.4))

   end subroutine dump_timings

end module timing_mod
!----------------------------------------------------------------------

