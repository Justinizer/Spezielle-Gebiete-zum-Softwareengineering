package Bean;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import javax.ejb.EJB;
import javax.ejb.Remote;
import javax.ejb.Stateful;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import Interface.HomeBeanRemote;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.SensorData;
import Model.SystemConfig;
import Model.Thing;
import Model.ThingType;
import Model.User;

/**
 * Session Bean implementation class BeanHome
 */
@Stateful
@Remote(HomeBeanRemote.class)
public class HomeBean implements HomeBeanRemote {

	@PersistenceContext
	EntityManager em;

	private boolean isLoggedin = false;
	private User user;

	/**
	 * Default constructor.
	 */
	public HomeBean() {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#test()
	 */
	@Override
	public String test() {
		return "Bean working";
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#createUser(java.lang.String,
	 * java.lang.String)
	 */
	@Override
	public boolean createUser(String email, String pw) {
		if (pw == null) {
			return false;
		}
		if (email == null) {
			return false;
		}
		if (!email.contains("@")) {
			return false;
		}
		if (pw.length() < 2) {
			return false;
		}
		/* parameter are valid, create an persist a new user */
		User u = new User(email, pw);
		em.persist(u);
		em.flush();
		return true;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#checkLogin(java.lang.String,
	 * java.lang.String)
	 */
	@Override
	public boolean checkLogin(String email, String pw) {
		if (pw == null) {
			user = null;
			isLoggedin = false;
			return false;
		}
		if (email == null) {
			user = null;
			isLoggedin = false;
			return false;
		}

		try {
			User result = em.createNamedQuery(User.LOGIN, User.class).setParameter("email", email)
					.setParameter("password", pw).getSingleResult();
			if (result != null) {
				isLoggedin = true;
				user = result;
				return true;
			}
		} catch (Exception e) {
		}
		return false;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getAllThings()
	 */
	@Override
	@SuppressWarnings("unchecked")
	public List<Thing> getAllThings() {
		if (isLoggedin) {
			try {
				return em.createNamedQuery(Thing.GET_ALL_THINGS).getResultList();
			} catch (Exception e) {

			}
		}
		return null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getSystemConfig()
	 */
	@Override
	public SystemConfig getSystemConfig() {
		return em.createNamedQuery(SystemConfig.GET_CONFIG, SystemConfig.class).getSingleResult();

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getUsername()
	 */
	@Override
	public String getUsername() {
		if (user == null) {
			return "";
		}
		return user.getEmail();
	}

	MqttClient client;

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#addData(Model.SensorData)
	 */
	@Override
	public void addData(SensorData s) {
		em.persist(s);
		em.flush();

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getAllDataForThing(int)
	 */
	@Override
	public List<SensorData> getAllDataForThing(int id) {
		if(!isLoggedin){
			return new ArrayList<SensorData>();
		}
		
		Thing t = em.find(Thing.class, id);
		// todo: check if user is allowed to see thing

		if (t == null) {
			return new ArrayList<SensorData>();
		}
		return t.getData();
	}
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#publish(int, java.lang.String)
	 */
	@Override
	public boolean publish(int id, String message) {	
			Thing t = em.find(Thing.class, id);
			return publish(t,message);
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#publish(Model.Thing, java.lang.String)
	 */
	@Override
	public boolean publish(Thing t, String message) {
		if(t == null){
			return false;
		}
		if(t.getType() == ThingType.Sensor){
			return false;
		}
		return publish(t.getMqttTopic(), message);
	}

	

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#publish(java.lang.String, java.lang.String)
	 */
	@Override
	public boolean publish(String t, String message) {
		if (!checkMqttClient()){
			return false;
		}
		try {						
			client.publish(t, new MqttMessage(message.getBytes()));
			System.out.println("done");
			return true;
		} catch (MqttException e) {
			e.printStackTrace();
		}
		return false;

	}

	/** 
	 * check if the mqtt client is ready, if not, connect it
	 * @return false = client cant be connected
	 */
	private boolean checkMqttClient() {

		try {
			if (client == null) {
				client = new MqttClient("tcp://" + getSystemConfig().getMqttServer(),
						"SHP" + new Random().nextInt(500000));
			}
			if (!client.isConnected()) {
				client.connect();
			}
		} catch (MqttException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return client.isConnected();
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#getAllAutomations()
	 */
	@Override
	public List<Automation> getAllAutomations() {
		if(!isLoggedin){
			return new ArrayList<Automation>(); 
		}
		List<Automation> autos = (List<Automation>) em.createNamedQuery(Automation.GET_ALL_AUTOMATIONS).getResultList();		
		return autos;
		
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#getAutomationById(int)
	 */
	@Override
	public Automation getAutomationById(int id) {	
		if(!isLoggedin){
			return new Automation();
		}
		return em.find(Automation.class, id);
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#addAutomation(java.lang.String)
	 */
	@Override
	public void addAutomation(String automationName) {
		if(!isLoggedin){
			return;
		}
		Automation auto = new Automation(automationName);
		em.persist(auto);
		em.flush();
		
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#addCondition(Model.Condition)
	 */
	@Override
	public void addCondition(Condition c) {
		if(!isLoggedin){
			return;
		}
		em.persist(c);
		em.flush();
		
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#getThingById(int)
	 */
	@Override
	public Thing getThingById(int id) {
		if(!isLoggedin){
			return null;
		}
		return em.find(Thing.class, id);
	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#addAction(Model.Action)
	 */
	@Override
	public void addAction(Action a) {
		if(!isLoggedin){
			return;
		}
		em.persist(a);
		em.flush();
		
	}
}
