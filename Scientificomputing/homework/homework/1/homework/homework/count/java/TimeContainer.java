import java.util.*;

/**
 * Acts as a storage class for the results of running
 * the Timing class. This provides a way of timing a 
 * number of methods a number of times and aggregating 
 * the results. These results can then be displayed using
 * the dumpTimings method. 
 */
public class TimeContainer
{
	HashMap timers;
	
	/**
	 * Represents a timing, or "class" of timings.
	 * In other words, stores the number of timings of
	 * a specific name, the sum of those timings, and the
	 * minimum and maximum timings. 
	 */ 
	class TimeNode
	{
		int count;
		long sum, min, max;
		
		//---------------------------------------------------------------------	
		
		/**
		 * Default constructor. Initializes variables
		 * to default values.
		 */
		public TimeNode()
		{
			count = 0;
			sum = 0;
			min = (long)1000000000; //big value
			max = 0;
		}
		
		//---------------------------------------------------------------------	
		
		/**
		 * Constructor. Initializes variables to specified
		 * values. 
		 *
		 * @param  countParam  int to assign to count. 
		 * @param  sumParam  int to assign to sum. 
		 * @param  minParam  int to assign to min. 
		 * @param  maxParam  int to assign to max. 
		 */
		public TimeNode(int countParam, long sumParam, long minParam, long maxParam)
		{
			count = countParam;
			sum = sumParam;
			min = minParam;
			max = maxParam;
		}   
	}

	//---------------------------------------------------------------------	

	/** 
	 * Default constructor. Initializes timers
	 * to a new HashMap.
	 */
	public TimeContainer()
	{   
		timers = new HashMap();
	}

	//---------------------------------------------------------------------	

   /**
    * Adds a timing result to this TimeContainer. In
	 * this case a timing is represented by a name and 
	 * an elapsed time. This is then added to the timings
	 * HashMap with the name as the key. 
	 *
    * @param  The name of the timing to add. 
    * @param  The elapsed time to add. 
    */
	public void addTiming(String name, long time)
	{
		//If a timing with this name is already in the 
		//hash, we update the appropriate values. 
		if(timers.containsKey(name))
		{
			TimeNode current = (TimeNode) timers.get(name);
		
			current.count++;
			current.sum += time;
			current.max = Math.max(current.max, time);
			current.min = Math.min(current.min, time);
		}
		//Otherwise, we add a new TimeNode to the hash. 
		else
		{
			timers.put(name, new TimeNode(1, time, time, time));	
		}	
	}
	
	//---------------------------------------------------------------------	

	/**
	 * Displays the results of the timings managed by this TimeContainer
	 * object. 
	 */
	public void dumpTimings(int n)
	{		
		System.out.println(getTimings(n));
	}


	//---------------------------------------------------------------------	


	/**
	 * Returns the results of the timings managed by this TimeContainer 
	 * object in a String. 
	 */
	public String getTimings(int n)
	{
	
		Iterator iter = timers.keySet().iterator();
		TimeNode current;
		
		StringBuffer timingString = new StringBuffer();
		
		
		timingString.append("Count"+"\t\t" + "Total (ms)"+"\t" + "N"+"\t"+"Mean (ms)"+"\n");
		
		
		while(iter.hasNext())
		{		
			String name = (String) iter.next();
			current = (TimeNode) timers.get(name);
			
			timingString.append(current.count+ "\t\t" + (double) current.sum +"\t\t"+(int) n + "\t\t" + ((double) current.sum / (double) current.count)  +"\n");
		}	
		
		return timingString.toString();
	}





}
