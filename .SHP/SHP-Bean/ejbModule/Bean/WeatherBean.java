package Bean;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Schedule;
import javax.ejb.Singleton;
import javax.ejb.Startup;

import com.arcusweather.forecastio.Weather;

import Interface.AutomationBeanRemote;
import Interface.HomeBeanRemote;

@Singleton
@Startup
public class WeatherBean {
	
	
	@EJB
	AutomationBeanRemote mb;
	
	@EJB
	HomeBeanRemote hb;
	
	@PostConstruct
	public void initIt() {
		onTimeout();
	}
	
	/**
	 * will be called every 30 minutes and executes the createWeather method
	 */
	@Schedule(second = "1", minute = "*/5", hour = "*")
	public  void onTimeout() {
	
		 Weather w = new Weather(hb.getSystemConfig().getLatitude(),hb.getSystemConfig().getLogitude());
		 mb.publish("/wetter", w.getWeatherName());
		 
		 
	}
}
