package Interface;

public abstract class ProtocolInterface {
	protected Receiver r = null;
	protected ProtocolInterface(Receiver r){
		this.r = r;
	}
	public abstract void connectToBroker() throws Exception;
	public abstract boolean publish(String t, String message);
	public abstract void listento(String t) throws Exception;
	public abstract void listento(String[] t) throws Exception;
}
