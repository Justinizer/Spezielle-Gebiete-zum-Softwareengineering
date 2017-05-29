package rest;

import javax.ejb.EJB;
import javax.enterprise.context.RequestScoped;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import Interface.HomeBeanRemote;



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
		return "{\"test\":" + "\"successful\"" + "}";
	}

	
	
	@Produces("application/json")
	@GET
	@Path("things")
	public String testThings() {
		//TODO: Implement listing all things		
		return "{\"test:)\":" + bh.test() + "}";
	}


}
