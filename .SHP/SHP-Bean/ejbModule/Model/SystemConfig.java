package Model;

import java.io.Serializable;
import javax.persistence.*;

/**
 * Entity implementation class for Entity: SystemConfig
 *
 */
@Entity
@NamedQueries({ @NamedQuery(name = SystemConfig.GET_CONFIG, query = "SELECT Z FROM SystemConfig z") })
public class SystemConfig implements Serializable {

	public static final String GET_CONFIG = "System.getConfig";
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Integer id;

	private String mqttServer;
	private int timerIntervall;

	private static final long serialVersionUID = 1L;

	public SystemConfig() {
		super();
	}

	public String getMqttServer() {
		return mqttServer;
	}

	public void setMqttServer(String mqttServer) {
		this.mqttServer = mqttServer;
	}

	public int getTimerIntervall() {
		return timerIntervall;
	}

	public void setTimerIntervall(int timerIntervall) {
		this.timerIntervall = timerIntervall;
	}

}
