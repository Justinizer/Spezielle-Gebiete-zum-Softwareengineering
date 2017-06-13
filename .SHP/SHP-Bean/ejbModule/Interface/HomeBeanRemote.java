package Interface;

import java.util.List;

import javax.ejb.Remote;
import javax.websocket.Session;
import javax.websocket.WebSocketContainer;

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
	
	public void addCondition(Condition c);
	
	public void addAction(Action a);
	
	public boolean deleteAction(int actionid);
	
	public boolean deleteCondition(int conditionid);
	
	/**
	 * update a given automation
	 * @param autoid the id
	 * @param name the new name. if name == null or name.length <1, name will be ignored and the old one is kept
	 * @param active
	 * @return
	 */
	public boolean updateAutomation(int autoid, String name, boolean active);
	
	
	
	
	
	
}
