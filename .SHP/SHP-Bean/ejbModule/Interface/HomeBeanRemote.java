package Interface;

import java.util.List;

import javax.ejb.Remote;

import Bean.Weather;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.SensorData;
import Model.SystemConfig;
import Model.Thing;

/**
 * @author Jonas
 *
 */
@Remote
public interface HomeBeanRemote {
	/**
	 * Test Method.
	 * @return "Bean working" if is working
	 */
	public String test();

	/**
	 * create a new user
	 * @param email must contain a @
	 * @param pw must be longer than 3 chars
	 * @return true = user created
	 */
	public boolean createUser(String email, String pw);

	/**
	 * check the login credentials of a user
	 * @param email
	 * @param pw
	 * @return true = correct
	 */
	public boolean checkLogin(String email, String pw);
	
	/**
	 * get all things for the current user
	 * @return
	 */
	public List<Thing> getAllThings();
	
	/**
	 * get a thing by id
	 * @param id
	 * @return null if id is invalid
	 */
	public Thing getThingById(int id);
	
	
	/**
	 * get the System Config Object
	 * @return
	 */
	public SystemConfig getSystemConfig();
	
	/**
	 * get the username of the currently loggedin user
	 * @return
	 */
	public String getUsername();
	
	/**
	 * add new sensor data to the database
	 * @param s
	 */
	public void addData(SensorData s);
	
	/**
	 * get all sensor data for a specific thing
	 * @param id the id of the thing
	 * @return the list of data
	 */
	public List<SensorData> getAllDataForThing(int id);
	
	/**
	 * Publish a Message to a thing. The mqtt Topic of the Thing will be used
	 * @param t the thing
	 * @param message
	 * @return true = success
	 */
	public boolean publish(Thing t, String message);
	
	/**
	 * Publish a Message to a thing.
	 * @param id of the thing. mqtt topic of thing will be used 
	 * @param message 
	 * @return true = success
	 */
	public boolean publish(int id, String message);	

	
	/**
	 * List all automations
	 * @return
	 */
	public List<Automation> getAllAutomations();

	/**
	 * get a specific automation by ID
	 * @param id
	 * @return
	 */
	public Automation getAutomationById(int id);
	
	/** 
	 * add a new Automation to the DB
	 * @param automationName
	 */
	public void addAutomation(String automationName);
	
	/**
	 * add a condition to a automation. The automation is set in the condition object
	 * @param c
	 */
	public void addCondition(Condition c);
	
	/**
	 * add an action to a automation. The automation is set in the Action object
	 * @param a
	 */
	public void addAction(Action a);
	
	/**
	 * delete an action
	 * @param actionid
	 * @return
	 */
	public boolean deleteAction(int actionid);
	
	/**
	 * delete a condition
	 * @param conditionid
	 * @return
	 */
	public boolean deleteCondition(int conditionid);
	
	/**
	 * update a given automation
	 * @param autoid the id
	 * @param name the new name. if name == null or name.length <1, name will be ignored and the old one is kept
	 * @param active
	 * @return
	 */
	public boolean updateAutomation(int autoid, String name, boolean active);
	
	
	/**
	 * Get the current weather for the configured point 
	 * @return
	 */
	public Weather getWeather();
	
	
	
	
}
