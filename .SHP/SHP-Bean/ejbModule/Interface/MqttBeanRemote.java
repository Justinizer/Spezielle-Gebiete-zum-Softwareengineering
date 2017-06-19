package Interface;

public interface MqttBeanRemote {
	
	/**
	 * just a test method to verify that the bean is reachable
	 */
	public void testi();
	
	/**
	 * publish a mqtt message
	 * @param t the topic of the message
	 * @param message the message
	 * @return false in case of error
	 */
	public boolean publish(String t, String message);
	
	/**
	 * reload the all the automations and all conditions
	 */
	public void reloadAutomations();
}
