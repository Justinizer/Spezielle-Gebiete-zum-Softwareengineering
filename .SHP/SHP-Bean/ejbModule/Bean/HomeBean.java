package Bean;

import java.util.ArrayList;
import java.util.List;

import javax.ejb.EJB;
import javax.ejb.Remote;
import javax.ejb.Stateful;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import Interface.HomeBeanRemote;
import Interface.MqttBeanRemote;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.ConditionType;
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

	@EJB
	MqttBeanRemote mb;

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
		if (!isLoggedin) {
			return new ArrayList<SensorData>();
		}

		Thing t = em.find(Thing.class, id);
		// todo: check if user is allowed to see thing

		if (t == null) {
			return new ArrayList<SensorData>();
		}
		return t.getData();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#publish(int, java.lang.String)
	 */
	@Override
	public boolean publish(int id, String message) {
		Thing t = em.find(Thing.class, id);

		return publish(t, message);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#publish(Model.Thing, java.lang.String)
	 */
	@Override
	public boolean publish(Thing t, String message) {
		if (t == null) {
			return false;
		}
		if (t.getType() == ThingType.Sensor) {
			System.out.println("publish to sensor -.-");
			return false;
		}
		return mb.publish(t.getMqttTopic(), message);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getAllAutomations()
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Automation> getAllAutomations() {
		if (!isLoggedin) {
			return new ArrayList<Automation>();
		}
		List<Automation> autos = (List<Automation>) em.createNamedQuery(Automation.GET_ALL_AUTOMATIONS).getResultList();
		return autos;

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getAutomationById(int)
	 */
	@Override
	public Automation getAutomationById(int id) {
		if (!isLoggedin) {
			return new Automation();
		}
		return em.find(Automation.class, id);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#addAutomation(java.lang.String)
	 */
	@Override
	public Automation addAutomation(String automationName,boolean active) {
		if (!isLoggedin) {
			return null;
		}
		Automation auto = new Automation(automationName);
		auto.setActive(active);
		em.persist(auto);
		em.flush();
		mb.reloadAutomations();
		System.out.println("bean auto added");
		return auto;

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#addCondition(Model.Condition)
	 */
	@Override
	public Automation addCondition(Condition c) {
		if (!isLoggedin) {
			return null;
		}
		em.persist(c);
		em.flush();
		mb.reloadAutomations();
		return c.getAutomation();
	}
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#updateAction(int, int, java.lang.String, java.lang.String)
	 */
	@Override
	public Automation updateAction(int actionid, int thingid, String name, String value){
		if(!isLoggedin){
			return null;
		}
		Action fromDB = em.find(Action.class, actionid);
		if(fromDB == null){
			return null;
		}
		Thing t = em.find(Thing.class, thingid);
		if(t == null){
			return null;
		}
		fromDB.setName(name);
		fromDB.setValue(value);
		fromDB.setThing(t);
		em.merge(fromDB);
		em.flush();
		mb.reloadAutomations();
		return getAutomationById(fromDB.getAutomation().getId());
	}
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#updateCondition(int, int, Model.ConditionType, java.lang.String)
	 */
	@Override
	public Automation updateCondition(int conditionID, int thingID, ConditionType type, String value){
		if(!isLoggedin){
			return null;
		}
		Condition fromDB = em.find(Condition.class, conditionID);
		if(fromDB != null){
			fromDB.setValue(value);
			Thing t = em.find(Thing.class, thingID);
			if(t == null){
				return null;
			}
			fromDB.setThing(t);
			fromDB.setType(type);
			em.merge(fromDB);
			em.flush();
			mb.reloadAutomations();
			return getAutomationById(fromDB.getAutomation().getId());
		}
		return null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#getThingById(int)
	 */
	@Override
	public Thing getThingById(int id) {
		if (!isLoggedin) {
			return null;
		}
		return em.find(Thing.class, id);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#addAction(Model.Action)
	 */
	@Override
	public Automation addAction(Action a) {
		if (!isLoggedin) {
			return null;
		}
		em.persist(a);
		em.flush();
		mb.reloadAutomations();
		return a.getAutomation();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#deleteAction(int)
	 */
	@Override
	public Automation deleteAction(int actionid) {
		if (!isLoggedin) {
			return null;
		}
		Action a = em.find(Action.class, actionid);
		if (a == null) {
			return null;
		} else {
			em.remove(a);
			em.flush();
			mb.reloadAutomations();
			return em.find(Automation.class, a.getAutomation().getId());
		}

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#deleteCondition(int)
	 */
	@Override
	public Automation deleteCondition(int conditionid) {
		if (!isLoggedin) {
			return null;
		}
		Condition c = em.find(Condition.class, conditionid);
		if (c == null) {
			return null;
		} else {
			em.remove(c);
			em.flush();
			mb.reloadAutomations();
			return em.find(Automation.class, c.getAutomation().getId());
		}

	}

	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#updateAutomation(int, java.lang.String, boolean)
	 */
	@Override
	public Automation updateAutomation(int autoid, String name, boolean active) {
		if (!isLoggedin) {
			return null;
		}
		Automation auto = em.find(Automation.class, autoid);
		if (auto == null) {
			return null;
		}

		if (name != null) {
			if (name.length() > 0) {
				auto.setName(name);
			}
		}

		auto.setActive(active);
		em.merge(auto);
		em.flush();
		mb.reloadAutomations();

		return em.find(Automation.class,autoid);
	}
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#deleteAutomation(int)
	 */
	public boolean deleteAutomation(int autoid){
		if (!isLoggedin) {
			return false;
		}
		try{
			Automation auto = em.find(Automation.class, autoid);
			for(Condition c:auto.getConditions()){
				em.remove(c);
			}
			for(Action a : auto.getActions()){
				em.remove(a);
			}
			em.remove(auto);
			em.flush();
			mb.reloadAutomations();
			return true;
		}catch(Exception e){
			
		}
		return false;
	}
	
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#getWeather()
	 */
	@Override
	public Weather getWeather(){
		return new Weather(52.2959618,8.9040964);
	}
	
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#addThing(java.lang.String, java.lang.String, Model.ThingType, java.lang.String)
	 */
	public Thing addThing(String name, String mqttTopic, ThingType type, String unit){
		if(!isLoggedin){
			return null;
		}
		Thing newThing = new Thing();
		newThing.setName(name);
		newThing.setMqttTopic(mqttTopic);
		newThing.setType(type);
		newThing.setUnit(unit);
		em.persist(newThing);
		em.flush();
		mb.reloadAutomations();
		return newThing;
	}
	
	/* (non-Javadoc)
	 * @see Interface.HomeBeanRemote#deleteThing(int)
	 */
	public boolean deleteThing(int Thingid){
		if(!isLoggedin){
			return false;
		}

		Thing toDel= em.find(Thing.class, Thingid);
		if(toDel == null){
			return false;
		}
		em.remove(toDel);
		return true;
	}
	



	
}
