package rest;

import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.enterprise.context.RequestScoped;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import Interface.HomeBeanRemote;



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
		System.out.println("TESTING!!");
		
		bh.createUser("aswd", "asd");
		
		
		return "{\"test:)\":" + bh.test() + "}";
	}


}
