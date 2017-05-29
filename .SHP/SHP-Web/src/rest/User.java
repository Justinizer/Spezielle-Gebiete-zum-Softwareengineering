package rest;

import javax.ejb.EJB;
import javax.enterprise.context.RequestScoped;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;

import Interface.HomeBeanRemote;



@RequestScoped
@Path("user")
public class User {

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
		if(bh.checkLogin(email, password)){
			return "{\"test:)\":" + "\"successful\"" + "}";
		}
		return "{\"status\":" + "\"FAILED\"" + "}";		
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


}
