import java.util.*;
import java.io.*;
class Counting
{
    
     Scanner in = new Scanner(System.in);
     int n = in.nextInt();
      
      TimeContainer container=new TimeContainer();
      Timing timer1=new Timing(container, "timer1");

      private float[] count = new float[n];
      public Counting() {
      int i,j,steps;
      steps=20;
      

      for (j=0; j<=steps; j++) {
          timer1.start();
      for (i=0; i < n; i++) {
           count[i] = i+1;
           System.out.println("count[" + i + "]= " + count[i]);
         }
         timer1.stop();
      }

      container.dumpTimings(n);
    }

	// accessor

    public static void main(String[] args) {
         System.out.println("Please Input a Integer:");
        Counting counting = new Counting();
    }
   

}
