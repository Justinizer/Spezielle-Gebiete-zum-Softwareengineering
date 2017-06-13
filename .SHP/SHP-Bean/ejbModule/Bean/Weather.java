package Bean;

import java.io.Serializable;
import java.util.HashMap;

import com.arcusweather.forecastio.ForecastIO;

/**
 * Represents the weather with position, name and icon
 *
 */
public class Weather implements Serializable {

	private static final long serialVersionUID = 6430273399244187457L;
	private String latitude;
	private String longitude;

	private String endLatitude;
	private String endLongitude;

	private String weather;
	private String weatherName;
	
		

	/**
	 * Create the Weather based on the output of the API
	 * 
	 * @param input
	 *            output of the ForecastIO api
	 */
	public Weather(String input) {
		praseInputData(input);		
	}
	
	
	/**
	 * Create the Weather
	 * @param lat latitude of the point
	 * @param lon longitude of the point
	 */
	public Weather(Double lat, Double lon){
		ForecastIO FIO = new ForecastIO("b04b450ee4f9ee7288dfb61015803985", 52.2959618,8.9040964);
		
		HashMap<String, String> requestParams = new HashMap<String, String>();

		// change units to something usable
		requestParams.put("units", "si");
		requestParams.put("userAgent", "Custom User Agent 1.0");
		FIO.setRequestParams(requestParams);
		FIO.makeRequest();
		praseInputData(FIO.getResponseString());
	}
	
	
	
	/**
	 * parse the output of the API and set the fields
	 * @param output the api output
	 */
	private void praseInputData(String output){
		int index = output.indexOf("\"latitude\":");
		index += 11;
		latitude = output.substring(index, output.indexOf(",", index));

		endLatitude = "" + (Double.parseDouble(latitude) + 0.5);

		index = output.indexOf("\"longitude\":");
		index += 12;
		longitude = output.substring(index, output.indexOf(",", index));

		endLongitude = "" + (Double.parseDouble(longitude) + 0.8);
		index = output.indexOf("icon");
		index += 7;
		weather = output.substring(index, output.indexOf("\"", index));
		setWeatherName(weather);
		if (weather.equals("rain")) {
			weather = "http://www.free-icons-download.net/images/rain4-90360.png";
		} else if (weather.equals("clear-day")) {
			weather = "http://www.free-icons-download.net/images/sun-icon-63131.png";
		} else if (weather.equals("partly-cloudy-day")) {
			weather = "http://www.free-icons-download.net/images/partly-cloudy-day-icon-61624.png";
		} else if (weather.equals("clear-night")) {
			weather = "http://www.free-icons-download.net/images/moon-and--stars-flag-icons-77050.png";
		} else if (weather.equals("snow")) {
			weather = "http://www.free-icons-download.net/images/wear-forecast-snow-flag-icons-88252.png";
		} else if (weather.equals("sleet")) {
			weather = "http://www.free-icons-download.net/images/sleet-icon-61467.png";
		} else if (weather.equals("wind")) {
			weather = "http://www.free-icons-download.net/images/winds-90290.png";
		} else if (weather.equals("fog")) {
			weather = "http://www.free-icons-download.net/images/fog11-91927.png";
		} else if (weather.equals("cloudy")) {
			weather = "http://www.free-icons-download.net/images/cloudy-flag-icons-84714.png";
		} else if (weather.equals("partly-cloudy-night")) {
			weather = "http://www.free-icons-download.net/images/partly-cloudy-night-icon-23102.png";
		}
		else {

			System.out.println(weather);
			weather = "http://www.free-icons-download.net/images/sad68-87171.png";
		}
	}

	public String getLatitude() {
		return latitude;
	}

	public void setLatitude(String latitude) {
		this.latitude = latitude;
	}

	public String getLongitude() {
		return longitude;
	}

	public void setLongitude(String longitude) {
		this.longitude = longitude;
	}

	public String getEndLatitude() {
		return endLatitude;
	}

	public void setEndLatitude(String endLatitude) {
		this.endLatitude = endLatitude;
	}

	public String getEndLongitude() {
		return endLongitude;
	}

	public void setEndLongitude(String endLongitude) {
		this.endLongitude = endLongitude;
	}

	public String getWeather() {
		return weather;
	}

	public void setWeather(String weather) {
		this.weather = weather;
	}

	public String getWeatherName() {
		return weatherName;
	}

	public void setWeatherName(String weatherName) {
		this.weatherName = weatherName;
	}

}
