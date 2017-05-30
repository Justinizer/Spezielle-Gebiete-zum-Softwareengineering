package Interface;

import java.util.List;

import javax.ejb.Remote;

import Model.SystemConfig;
import Model.Thing;

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
	 * get the System Config Object
	 * @return
	 */
	public SystemConfig getSystemConfig();
	
}
