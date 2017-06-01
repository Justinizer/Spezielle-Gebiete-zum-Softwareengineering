package Model;

import java.io.Serializable;
import javax.persistence.*;

/**
 * Entity implementation class for Entity: Condition
 *
 */
@Entity(name="\"Condition\"")

public class Condition implements Serializable {
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private int id;

	private String value;
	private boolean isAnd;
	
	@Enumerated(EnumType.STRING)
	private ConditionType type;
	
	@ManyToOne(fetch=FetchType.EAGER)
	@JoinColumn(name="Auto_ID")
	private Automation auto;
	
	@ManyToOne(fetch=FetchType.EAGER)
	@JoinColumn(name="Thing_ID")
	private Thing thing;
	

	private static final long serialVersionUID = 1L;

	public Condition() {
		super();
	}
	
	public Condition(Automation auto, ConditionType type,  Thing t, String value) {
		super();
		this.type = type;
		this.thing = t;
		this.value = value;
		this.auto = auto;
	}

	public int getId() {
		return this.id;
	}

	public boolean isAnd() {
		return isAnd;
	}

	public void setAnd(boolean isAnd) {
		this.isAnd = isAnd;
	}

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}
	
	public ConditionType getType(){
		return type;
	}
	
	public Thing getThing(){
		return thing;
	}

}
