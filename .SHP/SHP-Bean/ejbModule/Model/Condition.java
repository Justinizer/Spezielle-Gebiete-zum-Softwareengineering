package Model;

import java.io.Serializable;
import javax.persistence.*;

/**
 * Entity implementation class for Entity: Condition
 *
 */
@Entity(name = "\"Condition\"")

public class Condition implements Serializable {
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private int id;

	private String value;
	private boolean isAnd;

	@Enumerated(EnumType.STRING)
	private ConditionType type;

	@ManyToOne(fetch = FetchType.EAGER)
	@JoinColumn(name = "Auto_ID")
	private Automation auto;

	@ManyToOne(fetch = FetchType.EAGER)
	@JoinColumn(name = "Thing_ID")
	private Thing thing;

	private static final long serialVersionUID = 1L;

	public Condition() {
		super();
	}

	public Condition(Automation auto, ConditionType type, Thing t, String value) {
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

	public ConditionType getType() {
		return type;
	}

	public Thing getThing() {
		return thing;
	}

	public boolean fulfills(String newValue, String topic) {

		if (newValue == null) {
			return false;
		}
		if(thing.getMqttTopic().equals(topic)){
			thing.addData(newValue);
		} else {
			System.out.println("topic is different, getting last value");
			SensorData lastData = thing.getLastSensorData();
			if(lastData == null){
				return false;
			}
			newValue = lastData.getValue();
			
		}
		
		float newValueFloat = 0;
		float triggerFloat = 0;
		if (type != ConditionType.equal && type != ConditionType.notEqual) {			
			newValueFloat = Float.parseFloat(newValue);
			triggerFloat = Float.parseFloat(value);
		}
		

		switch (type) {
		case equal:
			return newValue.equals(value);
		case notEqual:
			return !newValue.equals(value);
		case smaller:
			return newValueFloat < triggerFloat;
		case smallerThan:
			return newValueFloat <= triggerFloat;
		case greater:
			return newValueFloat > triggerFloat;
		case greaterThan:
			return newValueFloat >= triggerFloat;
		}

		return false;
	}

}
