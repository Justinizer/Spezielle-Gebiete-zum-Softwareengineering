package Model;

import java.io.Serializable;
import javax.persistence.*;

/**
 * Entity implementation class for Entity: Action
 *
 */
@Entity

public class Action implements Serializable {

	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private int id;
	
	private String name;
	private String value;
	
	@ManyToOne(fetch=FetchType.EAGER)
	@JoinColumn(name="Thing_ID")
	private Thing thing;
	
	@ManyToOne(fetch=FetchType.EAGER)
	@JoinColumn(name="Auto_ID")
	private Automation auto;
	
	private static final long serialVersionUID = 1L;

	public Action() {
		super();
	}  
	public Action(String name, String value, Thing thing, Automation auto) {
		super();
		this.name = name;
		this.thing = thing;
		this.value = value;
		this.auto = auto;
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
	public String getValue() {
		return value;
	}
	public void setValue(String value) {
		this.value = value;
	}
	public Thing getThing(){
		return thing;
	}
   
}
