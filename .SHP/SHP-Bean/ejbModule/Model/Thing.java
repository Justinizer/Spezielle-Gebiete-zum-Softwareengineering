package Model;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;

/**
 * Entity implementation class for Entity: Thing
 *
 */
@Entity
@NamedQueries({ 
	@NamedQuery(name = Thing.GET_ALL_THINGS, query = "SELECT Z FROM Thing z"),
	@NamedQuery(name = Thing.GET_ALL_SENSORS, query = "SELECT Z FROM Thing z"),
	@NamedQuery(name = Thing.GET_ALL_ACTORS, query = "SELECT Z FROM Thing z")
})
public class Thing implements Serializable {

	public static final String GET_ALL_THINGS = "Thing.getAll";
	public static final String GET_ALL_SENSORS = "Thing.getAllSensors";
	public static final String GET_ALL_ACTORS = "Thing.getAllActors";
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Integer id;
	private String name;
	private String mqttTopic;
	private String unit; 
	
	
	@OneToMany(mappedBy = "thing", fetch=FetchType.LAZY, cascade = CascadeType.ALL )
	private List<SensorData> data;
	

    @Enumerated(EnumType.STRING)
    private ThingType type;
	
	private static final long serialVersionUID = 1L;

	public Thing() {
		super();
	}   
	public Integer getId() {
		return this.id;
	}

	public void setId(Integer id) {
		this.id = id;
	}   
	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}   
	public String getMqttTopic() {
		return this.mqttTopic;
	}

	public void setMqttTopic(String mqttTopic) {
		this.mqttTopic = mqttTopic;
	}   
	public List<SensorData> getData() {
		if(this.data == null){
			return new ArrayList<SensorData>();
		}
		return this.data;
	}
	
	public SensorData getLastSensorData(){
		if(this.data == null){
			return null;
		}
		
		if(this.data.size() == 0){
			return null;
		}
		return this.data.get(this.data.size()-1);
	}

	public void addData(SensorData data) {
		this.data.add(data);
	}
	
	public void addData(String data){
		this.data.add(new SensorData(data,this));
	}
   
	public ThingType getType(){
		return type;
	}
	public String getUnit() {
		if(unit == null)
			return "";
		if(unit.length() == 0)
			return "";
		
		return unit;
	}
	public void setUnit(String unit) {
		this.unit = unit;
	}
	public void setType(ThingType t){
		type = t;
	}
	
}
