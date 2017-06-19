package Bean;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Remote;
import javax.ejb.Singleton;
import javax.ejb.Startup;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.websocket.Session;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import Interface.HomeBeanRemote;
import Interface.MqttBeanRemote;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.SensorData;
import Model.SystemConfig;
import Model.Thing;

/**
 * Startup Bean for Mqtt Client. Connects to the Broker and adds Data to the DB
 */

@Startup
@Singleton
@Remote(MqttBeanRemote.class)
public class MqttBean implements MqttCallback, MqttBeanRemote {
	public static MqttClient client;
	
	/* kind of ugly to have it like that, but to make it nice a JMS needs to be added... 
	 * now the SHP-Web.websocket.WebSocket websocket endpoint adds and removes the sessions*/
	public static final Set<Session> clientSessions = Collections.synchronizedSet(new HashSet<Session>());
	

	@PersistenceContext
	EntityManager em;

	@EJB
	HomeBeanRemote hb;

	private Map<String, Thing> things = new HashMap<String, Thing>();

	private Map<String, List<Automation>> autos = new HashMap<String, List<Automation>>();

	public MqttBean() {

	}

	/**
	 * Initializes the mqtt client and connects to all topics
	 */
	@PostConstruct
	public void init() {
		System.out.println("----> MQTT BEAN STARTED <----");

		/* get the mqtt conifg */
		SystemConfig sc = hb.getSystemConfig();
		System.out.println(sc.getMqttServer());

		@SuppressWarnings("unchecked")
		List<Automation> autolist = em.createNamedQuery(Automation.GET_ALL_AUTOMATIONS).getResultList();

		/* build the map (topic -> automations) */
		autos = buildTopicAutomationMap(autolist);
		try {

			/* get all things form the database */
			@SuppressWarnings("unchecked")
			List<Thing> thingsList = (List<Thing>) em.createNamedQuery(Thing.GET_ALL_THINGS).getResultList();

			/* create and connect the mqttclient */
			if(client == null){
				client = new MqttClient("tcp://" + sc.getMqttServer(), "SHP" + new Random().nextInt(500000));
				client.connect();
				client.setCallback(this);
	
				/* subscribe to all necessary topics */
				for (Thing t : thingsList) {
					things.put(t.getMqttTopic(), t);
					client.subscribe(t.getMqttTopic());
				}
				
	
				System.out.println("!!!!CONNECTED!!!!");
			}
		} catch (MqttException e) {
			// TODO Auto-generated catch block
			System.out.println("!!!!MQTT EXCEPTION!!!!");
			System.out.println(e.toString());
		}

	}

	public void testi() {
		System.out.println("TEEEEEEEEEEEEEEEEEEEEST");
	}

	/**
	 * builds a map - mqtt topic to automation each subscribed mqtt topic is
	 * connected to at least one automation that needs to be checked, when new
	 * data is available on that topic
	 * 
	 * @param autolist
	 *            list of all automations
	 * @return
	 */
	private Map<String, List<Automation>> buildTopicAutomationMap(List<Automation> autolist) {
		Map<String, List<Automation>> map = new HashMap<String, List<Automation>>();

		/* loop all conditions in all automations */
		for (Automation a : autolist) {
			List<Condition> conditionList = a.getConditions();
			if (conditionList != null) {
				for (Condition c : a.getConditions()) {
					/* get the mqtt topic for the condition */
					String key = c.getThing().getMqttTopic();

					/* check if the topic is already in the map */
					if (map.containsKey(key)) {
						List<Automation> tmpList = map.get(key);
						/*
						 * check if the automation is already in the list, if
						 * not, it needs to be added
						 */
						if (!tmpList.contains(a)) {
							tmpList.add(a);
							map.put(key, tmpList);
						}
					} else {
						/* the topic was not in the map, it needs to be added */
						List<Automation> newList = new ArrayList<Automation>();
						newList.add(a);
						map.put(key, newList);
					}
				}
			}
		}
		return map;
	}

	/**
	 * callback for the mqtt client. Is called, when a new message for a
	 * subsribed topic is received
	 */
	@Override
	public void messageArrived(String topic, MqttMessage arg1) {
		System.out.println(topic + " " + new String(arg1.getPayload()));
		Thing databaseThing = things.get(topic);
		SensorData data = new SensorData(new String(arg1.getPayload()), databaseThing);
		
		/*
		 * warum ich den umweg über die hb gehe und nicht einfach
		 * em.persis(data) mache? weils nicht geht... namedquerrys gehen,
		 * persist nicht :(
		 */			
		try {
			hb.addData(data);
		} catch (Exception e) {
			System.out.println("MQTT BEAN: " + e.toString());
		}		

		handleWSSessions(databaseThing, data);
		
		
		List<Automation> affectedAutos = autos.get(topic);
		if (affectedAutos == null) {
			return;
		}

		/* check all affected automations, if they fulfill the conditions */
		for (Automation a : affectedAutos) {
			System.out.println("affected: " + a.getName());
			if (a.fulfilled(data.getValue(), topic)) {
				/* conditions are fulfilled: FIRE the actions */
				//System.out.println(a.getName() + "  firing!");
				for (Action action : a.getActions()) {
					publish(action.getThing().getMqttTopic(), action.getValue());
				}

			}
		}


	}
	
	
	/**
	 * update the websockets with the new data
	 * @param databaseThing the thing
	 * @param data the new data
	 */
	private void handleWSSessions(Thing databaseThing, SensorData data){
		ArrayList<Session> toDelete = new ArrayList<Session>();
		
		for (Session s : clientSessions){
			try {
				s.getBasicRemote().sendText("{\"id\":" + databaseThing.getId() + ", \"value\": \"" + data.getValue() + "\", \"type\":\"" + databaseThing.getType() + "\", \"unit\": \"" + databaseThing.getUnit() + "\"}");
			} catch (IOException e) {
				toDelete.add(s);
			}
		}
		
		for(Session s: toDelete){
			clientSessions.remove(s);
		}
	}

	/**
	 * check if the mqtt client is ready, if not, connect it
	 * 
	 * @return false = client cant be connected
	 */
	private boolean checkMqttClient() {

		try {
			if (client == null) {
				client = new MqttClient("tcp://" + hb.getSystemConfig().getMqttServer(),
						"SHP" + new Random().nextInt(500000));
			}
			if (!client.isConnected()) {
				client.connect();
			}
		} catch (MqttException e) {
			// TODO Auto-generated catch block
			System.out.println("MQTTBEAN CHECKMQTTCLIENT " + e.toString());
		}
		return client.isConnected();
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
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#publish(java.lang.String, java.lang.String)
	 */
	@Override
	public boolean publish(String t, String message) {
		if (!checkMqttClient()) {
			return false;
		}
		try {
			client.publish(t, new MqttMessage(message.getBytes()));
			//System.out.println("done");
			return true;
		} catch (MqttException e) {
			System.out.println("MQTT BEAN PUBLISH FAILED" + e.toString());
			//e.printStackTrace();
		}
		return false;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.MqttBeanRemote#reloadAutomations()
	 */
	@Override
	public void reloadAutomations() {
		init();
	}

}