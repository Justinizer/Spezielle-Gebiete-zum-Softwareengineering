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

public class Automation implements Serializable {

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
	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}   
	public int getId() {
		return this.id;
	}


   
}
