package rest;

import java.util.List;

import javax.ejb.EJB;
import javax.enterprise.context.RequestScoped;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import org.json.JSONObject;

import Interface.HomeBeanRemote;
import Model.Thing;



/**
 * @author Jonas
 *	Rest Test Class
 *  /test will return successful
 *  /test/things will list all things
 */
@RequestScoped
@Path("test")
public class Test {

	@EJB
	HomeBeanRemote bh;
	
	@Produces("application/json")
	@GET
	public String test() {
		System.out.println("TESTING!!");
		JSONObject json = new JSONObject();
		json.put("test", "successful");

		
		return json.toString();//"{\"test\":" + "\"successful\"" + "}";
	}

	
	
	@Produces("application/json")
	@GET
	@Path("things")
	public String testThings() {
		List<Thing> things = bh.getAllThings();		
		JSONObject json = new JSONObject();
		for(Thing t: things){
			JSONObject inner = new JSONObject();
			inner.put("type", t.getType());
			inner.put("name", t.getName());
			inner.put("mqtttopic", t.getMqttTopic());
			json.put(t.getId().toString(),inner );
		}
		
		
		//json.put("test", bh.test() );	
		return json.toString();
	}


}
