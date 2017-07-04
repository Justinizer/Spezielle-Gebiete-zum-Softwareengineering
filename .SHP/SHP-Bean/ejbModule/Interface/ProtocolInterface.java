package Interface;

/**
 * @author Jonas
 * Abstract class for protocol handler
 * A protocol handler connects to a "broker". It can send and receive messages from the nodes
 */
public abstract class ProtocolInterface {
	
	protected Receiver r = null;
	protected ProtocolInterface(Receiver r){
		this.r = r;
	}
	
	/**
	 * connect to the broker 
	 * @throws Exception
	 */
	public abstract void connectToBroker() throws Exception;
	
	/**
	 * publish a message to the broker
	 * @param t the "topic"
	 * @param message the message
	 * @return false = failed
	 */
	public abstract boolean publish(String t, String message);
	
	/**
	 * start listening to the topic
	 * @param t the topic
	 * @throws Exception
	 */
	public abstract void listento(String t) throws Exception;
	
	/**
	 * start listening to the following topics ONLY. 
	 * @param t
	 * @throws Exception
	 */
	public abstract void listento(String[] t) throws Exception;
	

}
