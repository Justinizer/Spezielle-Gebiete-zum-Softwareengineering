package rest;

import java.io.Serializable;
import java.util.List;

import javax.ejb.EJB;
import javax.enterprise.context.SessionScoped;
import javax.json.JsonObject;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;

import org.json.JSONObject;

import Interface.HomeBeanRemote;
import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.ConditionType;
import Model.SensorData;
import Model.Thing;

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
	@GET
	@Path("create/{email}/{password}")
	public String create(@PathParam("email") String email, @PathParam("password") String password) {
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
		JSONObject json = new JSONObject();
		for (Thing t : things) {
			JSONObject inner = new JSONObject();
			inner.put("id", t.getId());
			inner.put("type", t.getType());
			inner.put("name", t.getName());			
			inner.put("mqtttopic", t.getMqttTopic());
			inner.put("currentValue", helper.getCurrentValue(t));
			json.append("thing", inner);
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
		JSONObject json = new JSONObject();
		for (SensorData s : bh.getAllDataForThing(id)) {
			JSONObject inner = new JSONObject();
			inner.put("time", s.getTime());
			inner.put("value", s.getValue());
			inner.put("id", s.getId());
			json.append("data", inner);
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
	@Path("thing/{id}/{value}")
	public String changeState(@PathParam("id") int id, @PathParam("value") String value) {
		JSONObject json;

		if (bh.publish(id, value)) {
			json = helper.getSuccess();
		} else {
			json = helper.getFail();
		}
		return json.toString();
	}

	@Produces("application/json")
	@GET
	@Path("automation")
	public String getAutomations() {
		List<Automation> autos = bh.getAllAutomations();
		JSONObject json = helper.listAutomations(autos);		
		return json.toString();
	}

	@Produces("application/json")
	@GET
	@Path("automation/{name}")
	public String addAutomations(@PathParam("name") String autoname) {
		bh.addAutomation(autoname);
		return helper.getSuccess().toString();
	}
	
	@Produces("application/json")
	@GET
	@Path("automation/condition/{autoid}/{thingid}/{type}/{value}")
	public String addCondition(@PathParam("autoid") int autoid, @PathParam("thingid") int thingid,@PathParam("type") int type, @PathParam("value") String value) {
		Automation a = bh.getAutomationById(autoid);
		Thing t = bh.getThingById(thingid);
		ConditionType ct = ConditionType.getByIndex(type);
		if(a == null || t ==null || ct == null){
			return helper.getFail().toString();
		}
		Condition c = new Condition(a,ct,t,value);
		bh.addCondition(c);
		
		
		return helper.getSuccess().toString();
	}
	
	@Produces("application/json")
	@GET
	@Path("automation/action/{name}/{autoid}/{thing}/{value}")
	public String addCondition(@PathParam("name") String actionname, @PathParam("autoid") int auto,@PathParam("thing") int thing, @PathParam("value") String value) {
		Automation a = bh.getAutomationById(auto);
		Thing t = bh.getThingById(thing);
		if( t == null || a == null){
			return helper.getFail().toString();
		}
		Action action = new Action(actionname, value, t, a);
		bh.addAction(action);
		
		
		return helper.getSuccess().toString();
	}
	

}
