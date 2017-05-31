package rest;



import java.io.Serializable;
import java.util.List;

import javax.ejb.EJB;
import javax.enterprise.context.SessionScoped;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;

import org.json.JSONObject;

import Interface.HomeBeanRemote;
import Model.SensorData;
import Model.Thing;



@SessionScoped
@Path("gui")
public class Gui implements Serializable{

	private static final long serialVersionUID = 1L;
	@EJB
	HomeBeanRemote bh;
	
	/**
	 * REST Test Method
	 * @return always successful
	 */
	@Produces("application/json")
	@GET
	public String test() {
		System.out.println("TESTING!!");
		return "{\"test\":" + "\"successful\"" + "}";
	}

	
	
	/**
	 * Rest Method for login of a user
	 * @param email 
	 * @param password
	 * @return json object with status either \"TRUE\" or \"FALSE"\
	 */
	@Produces("application/json")
	@GET
	@Path("login/{email}/{password}")
	public String login(@PathParam("email") String email, @PathParam("password") String password) {
		JSONObject json = new JSONObject();
		if(bh.checkLogin(email, password)){
			json.put("status", "successful");
			return json.toString();
		}
		
		json.put("status", "failed");
		return json.toString();
	}
	
	
	/**
	 * Rest Method to create a user
	 * @param email the email address
	 * @param password the password
	 * @return json object with status either \"TRUE\" or \"FALSE"\
	 */
	@Produces("application/json")
	@GET
	@Path("create/{email}/{password}")
	public String create(@PathParam("email") String email, @PathParam("password") String password) {
		if(bh.createUser(email, password)){
			return "{\"status\":" + "\"True\"" + "}";
		}
		return "{\"status\":" + "\"False\"" + "}";		
	}
	
	/**
	 * lists all things and the current value of each thing
	 * @return
	 */
	@Produces("application/json")
	@GET
	@Path("things")
	public String getThings(){
		List<Thing> things = bh.getAllThings();		
		JSONObject json = new JSONObject();
		for(Thing t: things){
			JSONObject inner = new JSONObject();
			inner.put("id", t.getId());
			inner.put("type", t.getType());
			inner.put("name", t.getName());
			inner.put("mqtttopic", t.getMqttTopic());
			List<SensorData> datas = t.getData();
			
			if(datas.size() > 0){
				JSONObject lastValue = new JSONObject();
				SensorData data =  datas.get(datas.size()-1);
				lastValue.put("time", data.getTime());
				lastValue.put("value", data.getValue());
				inner.append("currentValue", lastValue);
			} else {
				inner.put("currentValue", "null");
			}			
			
			json.put(t.getId().toString(),inner );
		}
		
		return json.toString();		
	}
	
	
	
	@Produces("application/json")
	@GET
	@Path("things/{id}")
	public String getSensorData(@PathParam("id") String id) {
	
		JSONObject json = new JSONObject();
		try{
			
			for(SensorData s: bh.getAllDataForThing(Integer.parseInt(id))){
				JSONObject inner = new JSONObject();
				inner.put("time", s.getTime());
				inner.put("value", s.getValue());
				json.put(s.getId().toString(), inner);
			}
		}catch(Exception e){
			e.printStackTrace();
		}
		
		return json.toString();
	}
	
	
	
	
	


}
