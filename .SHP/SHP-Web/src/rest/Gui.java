package rest;

import java.io.Serializable;
import java.util.List;

import javax.ejb.EJB;
import javax.enterprise.context.SessionScoped;
import javax.ws.rs.DELETE;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;

import org.json.JSONArray;
import org.json.JSONObject;

import Bean.Weather;
import Interface.HomeBeanRemote;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.ConditionType;
import Model.SensorData;
import Model.Thing;
import Model.ThingType;

@SessionScoped
@Path("gui")
public class Gui implements Serializable {

	private static final long serialVersionUID = 1L;

	@EJB
	HomeBeanRemote bh;

	private GuiHelper helper = new GuiHelper();


	/**
	 * REST Test Method
	 * 
	 * @return always successful
	 */
	@Produces("application/json")
	@GET
	public String test() {
		System.out.println("TESTING!!");
		//System.out.println(WebSocket.clientSessions.size());
bh.test();
		
		return helper.getSuccess().toString();
	}

	/**
	 * Rest Method for login of a user
	 * 
	 * @param email
	 * @param password
	 * @return json object with status either \"TRUE\" or \"FALSE"\
	 */
	@Produces("application/json")
	@GET
	@Path("login/{email}/{password}")
	public String login(@PathParam("email") String email, @PathParam("password") String password) {
		JSONObject json= helper.getFail();
		try{
		if (bh.checkLogin(email, password)) {
			json = helper.getSuccess();
		} else {
			json = helper.getFail();
		}
		}catch(Exception e){
			
		}
		return json.toString();
	}

	@POST
	@Path("/login")
	@Produces("application/json")
	public String createUser(@FormParam("email") String email, @FormParam("password") String password) {
		System.out.println("got sth");
		System.out.println(email);
		System.out.println(email);
		JSONObject json;
		if (bh.checkLogin(email, password)) {
			json = helper.getSuccess();
		} else {
			json = helper.getFail();
		}

		return json.toString();
	}

	/**
	 * Rest Method to create a user
	 * 
	 * @param email
	 *            the email address
	 * @param password
	 *            the password
	 * @return json object with status either \"TRUE\" or \"FALSE"\
	 */
	@Produces("application/json")
	@PUT
	@Path("/login")
	public String create(@FormParam("email") String email, @FormParam("password") String password) {
		JSONObject json;
		if (bh.createUser(email, password)) {
			json = helper.getSuccess();
		} else {
			json = helper.getFail();
		}
		return json.toString();
	}

	/**
	 * lists all things and the current value of each thing
	 * 
	 * @return
	 */
	@Produces("application/json")
	@GET
	@Path("thing")
	public String getThings() {
		List<Thing> things = bh.getAllThings();
		JSONArray json = new JSONArray();
		if (things == null) {
			return helper.getFail().toString();
		}
		for (Thing t : things) {
			JSONObject inner = new JSONObject();
			inner.put("id", t.getId());
			inner.put("type", t.getType());
			inner.put("name", t.getName());
			inner.put("unit", t.getUnit());
			inner.put("mqtttopic", t.getMqttTopic());
			inner.put("currentValue", helper.getCurrentValue(t));
			json.put(inner);
		}

		return json.toString();
	}

	@Produces("application/json")
	@GET
	@Path("thing/actor/{email}/{password}")
	public String getAllActors(@PathParam("email") String email, @PathParam("password") String password) {
		bh.checkLogin(email, password);
		List<Thing> things = bh.getAllThings();
		JSONArray json = new JSONArray();
		for (Thing t : things) {
			if (t.getType() == ThingType.AnalogActor || t.getType() == ThingType.DigitalActor) {
				JSONObject inner = new JSONObject();
				inner.put("name", t.getName());
				inner.put("id", t.getId());
				json.put(inner);
			}
		}

		return json.toString();
	}

	/**
	 * get all sensor data for a specific thing
	 * 
	 * @param id
	 *            the id of the thing
	 * @return all the data
	 */
	@Produces("application/json")
	@GET
	@Path("thing/{id}")
	public String getSensorData(@PathParam("id") int id) {
		JSONArray json = new JSONArray();
		for (SensorData s : bh.getAllDataForThing(id)) {
			JSONObject inner = new JSONObject();
			inner.put("time", s.getTime());
			inner.put("value", s.getValue());
			inner.put("id", s.getId());
			json.put(inner);
		}
		return json.toString();
	}

	/**
	 * get all sensor data for a specific thing
	 * 
	 * @param id
	 *            the id of the thing
	 * @return all the data
	 */
	@Produces("application/json")
	@POST
	@Path("thing")
	public String changeState(@FormParam("id") int id, @FormParam("value") String value) {
		JSONObject json = helper.getFail();
		try {
			if (bh.publish(id, value)) {
				json = helper.getSuccess();
			} else {
				json = helper.getFail();
			}
		} catch (Exception e) {

		}
		return json.toString();
	}

	/**
	 * enable the lambda function of alexa to change the state of a device
	 * 
	 * @param id
	 *            the id of the thing
	 * @param value
	 *            the new value of the thing
	 * @param username
	 *            the username
	 * @param password
	 *            the password
	 * 
	 * @return all the data
	 */
	@Produces("application/json")
	@GET
	@Path("thing/{id}/{value}/{username}/{password}")
	public String changeStateForAlexa(@PathParam("id") int id, @PathParam("value") String value,
			@PathParam("username") String user, @PathParam("password") String password) {
		JSONObject json = helper.getFail();
		if (bh.checkLogin(user, password)) {
			if (bh.publish(id, value)) {
				json = helper.getSuccess();
			}
		}
		return json.toString();

	}

	/**
	 * get a list of all automation, with the actions and conditions
	 * 
	 * @return
	 */
	@Produces("application/json")
	@GET
	@Path("automation")
	public String getAutomations() {
		List<Automation> autos = bh.getAllAutomations();
		JSONArray json = helper.listAutomations(autos);
		return json.toString();
	}

	/**
	 * create a new automation
	 * 
	 * @param autoname
	 *            the name of the automation
	 * @return
	 */
	@Produces("application/json")
	@PUT
	@Path("automation")
	public String addAutomations(@FormParam("name") String autoname) {
		Automation a= bh.addAutomation(autoname);

		if(a != null){
			return helper.getOneAutomation(a).toString();
		}
		return helper.getFail().toString();
	}

	/**
	 * add a new condition to an automation
	 * 
	 * @param autoid
	 *            the id of the automation
	 * @param thingid
	 *            the id of the thing
	 * @param type
	 *            the type of the condition (0-5) -> Seen conditiontype
	 * @param value
	 *            the value that must be reached
	 * @return
	 */
	@Produces("application/json")
	@PUT
	@Path("automation/condition")
	public String addCondition(@FormParam("autoid") int autoid, @FormParam("thingid") int thingid,
			@FormParam("type") int type, @FormParam("value") String value) {
		Automation a = bh.getAutomationById(autoid);
		Thing t = bh.getThingById(thingid);
		ConditionType ct = ConditionType.getByIndex(type);
		if (a == null || t == null || ct == null) {
			return helper.getFail().toString();
		}
		Condition c = new Condition(a, ct, t, value);
		Automation an= bh.addCondition(c);
		if(an != null){
			return helper.getOneAutomation(an).toString();
		}
		return helper.getFail().toString();
	}

	/**
	 * add an action to an automation
	 * 
	 * @param actionname
	 *            name of the action
	 * @param auto
	 *            id of the automation
	 * @param thing
	 *            id of the thing
	 * @param value
	 *            value that should be set, when fireing the action
	 * @return
	 */
	@Produces("application/json")
	@PUT
	@Path("automation/action")
	public String addAction(@FormParam("name") String actionname, @FormParam("autoid") int auto,
			@FormParam("thing") int thing, @FormParam("value") String value) {
		Automation a = bh.getAutomationById(auto);
		Thing t = bh.getThingById(thing);
		if (t == null || a == null) {
			return helper.getFail().toString();
		}
		Action action = new Action(actionname, value, t, a);
		Automation an= bh.addAction(action);

		if(an != null){
			return helper.getOneAutomation(an).toString();
		}
		return helper.getFail().toString();
	}

	/**
	 * delete an action
	 * 
	 * @param actionid
	 *            the id of the action
	 * @return success = done
	 */
	@Produces("application/json")
	@DELETE
	@Path("automation/action/{actionid}")
	public String deleteAction(@PathParam("actionid") int actionid) {
		Automation a =bh.deleteAction(actionid);
		if(a != null){
			return helper.getOneAutomation(a).toString();
		}
		return helper.getFail().toString();
	}

	/**
	 * delete an condition
	 * 
	 * @param conditionid
	 *            the id of the action
	 * @return success = done
	 */
	@Produces("application/json")
	@DELETE
	@Path("automation/condition/{conditionid}")
	public String deleteCondtition(@PathParam("conditionid") int conditionid) {
		Automation a = bh.deleteCondition(conditionid);
		if(a != null){
			return helper.getOneAutomation(a).toString();
		}
		return helper.getFail().toString();
	}

	/**
	 * update a given automation
	 * 
	 * @param autoid
	 *            the id of the automation
	 * @param name
	 *            the new name. if name == null or name.length <1, name will be
	 *            ignored and the old one is kept
	 * @param active
	 * @return
	 */
	@Produces("application/json")
	@POST
	@Path("automation")
	public String updateAutomation(@FormParam("automationid") int autoid, @FormParam("name") String name,
			@FormParam("active") boolean active) {
		Automation a  =bh.updateAutomation(autoid, name, active);
		if (a != null) {
			return helper.getOneAutomation(a).toString();
		} else {
			return helper.getFail().toString();
		}
	}
	
	/**
	 * get all conditions types with descriptions 
	 * @return
	 */
	@Produces("application/json")
	@GET
	@Path("automation/conditionTypes")
	public String getConditionTypes(){
		ConditionType[] cons= ConditionType.getAll();
		JSONArray ob = new JSONArray();
		for(ConditionType c:cons){
			JSONObject inner = new JSONObject();
			inner.put("id", ConditionType.getAsInt(c));
			inner.put("info", ConditionType.getAsText(c));
			ob.put(inner);
		}
		
		return ob.toString();
		
	}
	
	@Produces("application/json")
	@GET
	@Path("weather")
	public String getWeather(){
		Weather wr = bh.getWeather();
		JSONObject json = new JSONObject();
		json.put("name", wr.getWeatherName());
		json.put("img", wr.getWeather());
		return json.toString();
		
		
	}
	

}
