package Model;

import java.io.Serializable;
import java.lang.String;
import java.util.List;

import javax.persistence.*;

/**
 * Entity implementation class for Entity: Automation
 *
 */

@Entity
@NamedQueries({ 
	@NamedQuery(name = Automation.GET_ALL_AUTOMATIONS, query = "SELECT Z FROM Automation z")})
public class Automation implements Serializable {
	public static final String GET_ALL_AUTOMATIONS = "Auto.getAll";
	
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private int id;
	
	@OneToMany(mappedBy = "auto", fetch=FetchType.EAGER)
	private List<Condition> conditions;
	
	@OneToMany(mappedBy = "auto", fetch=FetchType.EAGER)
	private List<Action> actions;
	
	private boolean active;
	
	private String name;
	
	private static final long serialVersionUID = 1L;

	public Automation() {
		super();
	}  
	
	public Automation(String name) {
		super();
		this.name = name;
	}   
	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}   
	public int getId() {
		return this.id;
	}

	
	public List<Condition> getConditions(){
		return conditions;
	}
	
	
	public List<Action> getActions(){
		return actions;
	}
	
	
	public void addCondition(Condition c){
		conditions.add(c);
	}
	
	public void addAction(Action a){
		actions.add(a);
	}
	
	
	/**
	 * check if the automation with all it conditions is fulfilled
	 * will not fire the actions
	 * @param newValue the new received value
	 * @param topic on the topic
	 * @return true = fulfilled
	 */
	public boolean fulfilled(String newValue, String topic){
		/* if the automation is not active, the conditions do not need to be checked */
		if(!active){
			System.out.println("automation is not active");
			return false;
		}
		
		boolean allTrue = true;
		System.out.println("in automation " + id + " conditions:" + conditions.size());
		
		/* check all conditions of the automation */
		for(Condition c:conditions){
			System.out.println("checking condition " + c.getId());
			
			/* if one condition is not fulfilled, the whole automation is NOT */
			if(!c.fulfills(newValue, topic)){
				System.out.println("is NOT fulfilled");
				allTrue = false;
			} else {
				System.out.println("is fulfilled");
			}
			
		}
		
		/* just for debugging */
		if(allTrue){
			System.out.println("AUTOMATION FULFILLED! WILL FIRE!");
		} else {
			System.out.println("Automation NOT fulfilled");
		}
		
		
		return allTrue;
	}

	public boolean isActive() {
		return active;
	}

	public void setActive(boolean active) {
		this.active = active;
	}

   
}
