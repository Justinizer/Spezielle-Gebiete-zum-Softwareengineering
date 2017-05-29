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
	private float value;
	
	@ManyToOne(fetch=FetchType.EAGER)
	@JoinColumn(name="Thing_ID")
	private Thing thing;
	
	
	private static final long serialVersionUID = 1L;

	public SensorData() {
		super();
	}   
	public Integer getId() {
		return this.id;
	}

	public void setId(Integer id) {
		this.id = id;
	}   
	public int getTime() {
		return this.time;
	}

	public void setTime(int time) {
		this.time = time;
	}   
	public float getValue() {
		return this.value;
	}

	public void setValue(float value) {
		this.value = value;
	}
   
}
