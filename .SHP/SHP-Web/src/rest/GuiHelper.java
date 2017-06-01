package rest;

import java.util.List;

import org.json.JSONObject;

import Model.SensorData;
import Model.Thing;

public class GuiHelper {

	private JSONObject success = new JSONObject();
	private JSONObject fail = new JSONObject();
	
	
	public GuiHelper(){
		success.put("status", "successful");
		fail.put("status", "fail");
	}
	
	/**
	 * get the current value of a thing. 
	 * @param t the thing
	 * @return if no data is available, the object is empty
	 */
	public JSONObject getCurrentValue(Thing t) {
		List<SensorData> datas = t.getData();
		JSONObject lastValue = new JSONObject();
		if (datas.size() == 0) {
			return lastValue;
		}
		SensorData data = datas.get(datas.size() - 1);
		lastValue.put("time", data.getTime());
		lastValue.put("value", data.getValue());
		return lastValue;
	}
	
	public JSONObject getSuccess(){
		return success;
	}
	
	public JSONObject getFail(){
		return fail;
	}
}
