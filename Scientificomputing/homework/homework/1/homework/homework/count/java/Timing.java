import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Represents a simple timer that can be used to calculate the time
 * that has elapsed between two points in a Java program. Individual
 * Timing objects can be stored in a TimeContainer class for easy storage 
 * and processing. 
 * 
 * A timer can be used in the following way:
 *
 *  Timing timer = new Timer(<TimeContainer container>, <String name>);
 *  timer.start();
 *  timer.begin(); // same as start
 *  // Insert code to time here
 *  timer.stop();
 *  timer.end(); // same as stop
 *  //maybe retrieve results here 
 *  timer.reset();
 * 
 * The timing results can be retrieved by using timer.getTime()
 * or by using container.dumpTimings() at a later time. 
 */ 
public class Timing 
{
	private long startTime, endTime;
	private TimeContainer container;
	private String name;

	//---------------------------------------------------------------------	
	
	/**
	 * Default constructor. Creates a standalone timer that
	 * is not associated with a TimeContainer object. 
	 */
	public Timing() 
	{
		reset();
		name = null;
		container = null;
		
	}

	//---------------------------------------------------------------------	
	
	/**
	 * Constructor. Creates a timer that will be added to the specified 
	 * TimeContainer object. This provides an easy way of storing and displaying
	 * the timing results. 
	 *
	 * @param  containerParam  The TimeContainer object to associate this timer with
	 * @param  nameParam  The name of this timer. 
	 */
	public Timing(TimeContainer containerParam, String nameParam)
	{
		reset();
		container = containerParam;
		name = nameParam;

	}

	//---------------------------------------------------------------------	

	/**
	 * Starts the timer. 
	 */
	public void start()
	{
		startTime = System.currentTimeMillis();
	}

	public void begin()
	{
		start();
	}
	//---------------------------------------------------------------------	

	/**
	 * Stops the timer. If the timer was created with an
	 * associated TimeContainer object, the timer is added
	 * to that object. 
	 */
	public void stop()
	{
		endTime = System.currentTimeMillis();
	
		if(name != null)
		{
			container.addTiming(name, getTime());
		}
	}

	public void end()
	{
		stop();
	}

	//---------------------------------------------------------------------	

	/**
	 * Resets the timer by clearing the
	 * startTime and endTime.
	 */
	public void reset()
	{
		startTime = 0;
		endTime   = 0;
	}

	//---------------------------------------------------------------------	

	/**
	 * Returns the time elapsed in milliseconds
	 * between the last calls of start and stop.
	 *
	 * @return  The elapsed time in milliseconds.
	 */
	public long getTime()
	{
		return (endTime-startTime);
	}
	
	//---------------------------------------------------------------------	
	
	/**
	 * Returns a timestamp using the default format. This
	 * method is considerably slower than the other timing 
	 * methods, so it should be used with caution. 
	 *
	 * @returns  The current date and time in the default format
	 */
	public static String getTimestamp()
	{   
		SimpleDateFormat dateFormat = new SimpleDateFormat();
		return dateFormat.format(new Date());
	}
}
