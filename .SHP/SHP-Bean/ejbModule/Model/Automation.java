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

   
}
