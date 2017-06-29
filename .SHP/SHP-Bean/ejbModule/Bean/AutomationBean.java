package Bean;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Remote;
import javax.ejb.Singleton;
import javax.ejb.Startup;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.websocket.Session;


import Interface.HomeBeanRemote;
import Interface.ProtocolInterface;
import Interface.Receiver;
import Interface.AutomationBeanRemote;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.SensorData;
import Model.Thing;

/**
 * Startup Bean for Mqtt Client. Connects to the Broker and adds Data to the DB
 */

@Startup
@Singleton
@Remote(AutomationBeanRemote.class)
public class AutomationBean implements AutomationBeanRemote, Receiver {

	/*
	 * kind of ugly to have it like that, but to make it nice a JMS needs to be
	 * added... That would create a lot of overhead. Maybe in version 2.0.
	 * now the SHP-Web.websocket.WebSocket websocket endpoint adds and
	 * removes the sessions
	 */
	public static final Set<Session> clientSessions = Collections.synchronizedSet(new HashSet<Session>());

	@PersistenceContext
	EntityManager em;

	@EJB
	HomeBeanRemote hb;

	private Map<String, Thing> things = new HashMap<String, Thing>();

	private Map<String, List<Automation>> autos = new HashMap<String, List<Automation>>();

	private ProtocolInterface mqtt;

	public AutomationBean() {

	}

	/**
	 * called at the startup of the Server Initializes the mqtt client and
	 * connects to all topics
	 */
	@PostConstruct
	public void init() {
		System.out.println("----> MQTT BEAN STARTED <----");
		buildTopicAutomationMap();

		mqtt = new MqttBean(this);
		things = buildThingsMap();
		try {
			mqtt.connectToBroker();
			Set<String> temp = things.keySet();
			mqtt.listento(temp.toArray(new String[temp.size()]));

		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("MQTT BEAN START: " + e.toString());
		}

	}

	/**
	 * builds a map - mqtt topic to automation each subscribed mqtt topic is
	 * connected to at least one automation that needs to be checked, when new
	 * data is available on that topic
	 * 
	 */
	private void buildTopicAutomationMap() {
		@SuppressWarnings("unchecked")
		List<Automation> autolist = em.createNamedQuery(Automation.GET_ALL_AUTOMATIONS).getResultList();
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
		autos = map;
	}

	/**
	 * build a hashmap to connect a topic to a thing
	 * when a message is received, with this map, the topic can be mapped to the thing
	 * @return
	 */
	private Map<String, Thing> buildThingsMap() {

		Map<String, Thing> things = new HashMap<String, Thing>();
		@SuppressWarnings("unchecked")
		List<Thing> thingsList = (List<Thing>) em.createNamedQuery(Thing.GET_ALL_THINGS).getResultList();
		things.clear();
		for (Thing t : thingsList) {
			things.put(t.getMqttTopic(), t);

		}
		return things;
	}

	/**
	 * update the websockets with the new data
	 * 
	 * @param databaseThing
	 *            the thing
	 * @param data
	 *            the new data
	 */
	private void handleWSSessions(Thing databaseThing, SensorData data) {
		ArrayList<Session> toDelete = new ArrayList<Session>();

		for (Session s : clientSessions) {
			try {
				s.getBasicRemote()
						.sendText("{\"id\":" + databaseThing.getId() + ", \"value\": \"" + data.getValue()
								+ "\", \"type\":\"" + databaseThing.getType() + "\", \"unit\": \""
								+ databaseThing.getUnit() + "\"}");
			} catch (IOException e) {
				toDelete.add(s);
			}
		}

		for (Session s : toDelete) {
			clientSessions.remove(s);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#publish(java.lang.String, java.lang.String)
	 */
	@Override
	public boolean publish(String t, String message) {
		return mqtt.publish(t, message);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.MqttBeanRemote#reloadAutomations()
	 */
	@Override
	public void reloadAutomations() {
		buildTopicAutomationMap();

		Set<String> temp = things.keySet();
		try {
			mqtt.listento(temp.toArray(new String[temp.size()]));
		} catch (Exception e) {
			System.out.println("RELOAD AUTOMATION " + e.getMessage());
		}

	}

	public void testi() {
		System.out.println("TEEEEEEEEEEEEEEEEEEEEST");
	}

	@Override
	public String getBrokerAddress() {
		return hb.getSystemConfig().getMqttServer();
	}

	@Override
	public void gotMessage(String message, String sender) {
		System.out.println(sender + " " + message);
		Thing databaseThing = things.get(sender);
		SensorData data = new SensorData(message.replace("\\", "\\\\"), databaseThing);

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

		List<Automation> affectedAutos = autos.get(sender);
		if (affectedAutos == null) {
			return;
		}

		/* check all affected automations, if they fulfill the conditions */
		for (Automation a : affectedAutos) {
			System.out.println("affected: " + a.getName());
			if (a.fulfilled(data.getValue(), sender)) {
				/* conditions are fulfilled: FIRE the actions */
				for (Action action : a.getActions()) {
					publish(action.getThing().getMqttTopic(), action.getValue());
				}

			}
		}

	}

}