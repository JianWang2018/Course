
program test
   use timing_mod
   
   integer n, steps
   !type(times) :: t1, t2

   real, dimension(100000) :: count

   n=100000
   steps=20
   ! Not the best interface. It would be better to do start_timer('full')

   call setup_timers(2)
   call setup_timer(2,'loop')


   do j=1,steps
   call start_timer(2)
      do i=1,n
         count(i)=i
         print*,'count(',i,')= ',count(i)
      end do
   call stop_timer(2)
   end do
   
   call dump_timings()
   end program test

