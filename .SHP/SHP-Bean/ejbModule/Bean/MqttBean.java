package Bean;

import java.util.HashSet;
import java.util.Random;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import Interface.ProtocolInterface;
import Interface.Receiver;

/**
 * @author Jonas
 * ProtocolInterface implementation for MQTT
 */
public class MqttBean extends ProtocolInterface implements MqttCallback {

	private  MqttClient client;
	private HashSet<String> topics = new HashSet<String>();

	public MqttBean(Receiver r) {
		super(r);
	}
	
	/* (non-Javadoc)
	 * @see Interface.ProtocolInterface#connectToBroker()
	 */
	@Override
	public void connectToBroker() throws Exception {
		client = connectMQTT(r.getBrokerAddress());
		for (String t : topics)
			client.subscribe(t);

	}

	/**
	 * connect a new mqtt client to the broker, and register the callback
	 * 
	 * @param sc
	 *            the broker ip
	 * @return the connected client
	 * @throws MqttException
	 */
	private MqttClient connectMQTT(String sc) throws MqttException {

		MqttClient client = new MqttClient("tcp://" + sc, "SHP" + new Random().nextInt(500000));
		client.connect();
		client.setCallback(this);
		System.out.println("!!!!CONNECTED!!!!");
		return client;

	}


	/* (non-Javadoc)
	 * @see Interface.ProtocolInterface#publish(java.lang.String, java.lang.String)
	 */
	@Override
	public boolean publish(String t, String message) {
		if (!checkMqttClient()) {
			return false;
		}
		try {
			client.publish(t, new MqttMessage(message.getBytes()));
			return true;
		} catch (MqttException e) {
			System.out.println("MQTT BEAN PUBLISH FAILED" + e.toString());
		}
		return false;
	}

	/**
	 * check if the mqtt client is ready, if not, connect it
	 * 
	 * @return false = client cant be connected
	 */
	private boolean checkMqttClient() {

		if(client != null)
		{
			if(client.isConnected())
				return true;
		}
		try{
			connectToBroker();
		}catch(Exception m){
			return false;
		}
		
		return client.isConnected();
	}
	
	
	/* (non-Javadoc)
	 * @see Interface.ProtocolInterface#listento(java.lang.String[])
	 */
	@Override
	public void listento(String[] t) throws Exception {
		/* listen only to the new topics! */
		for(String old: topics){
			client.unsubscribe(old);
		}
		
		topics.clear();
		for (String m : t) {
			topics.add(m);
			if (client != null) {
				client.subscribe(m);

			}
		}
	}

	/* (non-Javadoc)
	 * @see Interface.ProtocolInterface#listento(java.lang.String)
	 */
	@Override
	public void listento(String t) throws Exception {
		if (topics.contains(t)) {
			return;
		}
		topics.add(t);
		if (client == null) {
			return;
		}
		client.subscribe(t);

	}

	@Override
	public void connectionLost(Throwable arg0) {
		/* if the connection is lost, wait two seconds, and try to reconnect */
		System.out.println("DC!");
		sleep2k();
		while (!client.isConnected()) {
			System.out.println("connect...");
			try {
				client.connect();
			} catch (MqttException e) {
				System.out.println("connect Failed!");
				sleep2k();
			}
		}
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken arg0) {
		// TODO Auto-generated method stub

	}

	/**
	 * callback for the mqtt client. Is called, when a new message for a
	 * subsribed topic is received
	 */
	@Override
	public void messageArrived(String topic, MqttMessage arg1) {
		r.gotMessage(new String(arg1.getPayload()), topic);

	}

	/**
	 * Sleep 2 seconds
	 */
	private void sleep2k() {
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
		}
	}

	



}
