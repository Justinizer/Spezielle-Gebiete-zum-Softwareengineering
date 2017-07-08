package Model;

import java.io.Serializable;
import java.lang.Integer;
import javax.persistence.*;


/**
 * Entity implementation class for Entity: SensorData
 *
 */
@Entity

public class SensorData implements Serializable {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Integer id;
	private int time;
	private String value;
	
	@ManyToOne(fetch=FetchType.EAGER)
	@JoinColumn(name="Thing_ID")
	private Thing thing;
	
	
	private static final long serialVersionUID = 1L;

	public SensorData() {
		super();
	}   
	
	public SensorData(String value, Thing t){
		time = (int) (System.currentTimeMillis() / 1000L);
		this.value = value.replace("\\", "\\\\");
		this.thing = t;
	}
	
	public Integer getId() {
		return this.id;
	}

	public void setId(Integer id) {
		this.id = id;
	}   
	public long getTime() {
		return this.time;
	}

	public void setTime(int time) {
		this.time = time;
	}   
	public String getValue() {
		return this.value;
	}

	public void setValue(String value) {
		this.value = value;
	}
	
	@Override
	public String toString(){

		return "SensorData time: " + time + " value: " + value;
	}
	
	
}
