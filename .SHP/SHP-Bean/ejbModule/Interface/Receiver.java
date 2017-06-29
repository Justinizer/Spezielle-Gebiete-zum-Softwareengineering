package Interface;


/**
 * @author Jonas
 * A Receiver has a Instance of ProtocolInterface
 * Receiver is the callback for the ProtocolInterface
 *
 */
public interface Receiver {
	
	/**
	 * get the broker URL / IP
	 * @return the url / ip
	 */
	public String getBrokerAddress();
	
	/**
	 * Message received
	 * @param message the message
	 * @param sender the sender
	 */
	public void gotMessage(String message, String sender);
}
