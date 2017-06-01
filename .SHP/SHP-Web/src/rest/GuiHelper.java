package rest;

import java.util.List;

import org.json.JSONObject;

import Model.Action;
import Model.Automation;
import Model.Condition;
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
		SensorData data = t.getLastSensorData();
		JSONObject lastValue = new JSONObject();
		if (data == null) {
			return lastValue;
		}		
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
	
	public JSONObject listAutomations(List<Automation>autos){
		JSONObject json = new JSONObject();
		for (Automation auto : autos) {

			JSONObject inner = new JSONObject();
			inner.put("name", auto.getName());
			inner.put("id", auto.getId());
			
			for (Condition c : auto.getConditions()) {
				JSONObject condition = new JSONObject();
				condition.put("id", c.getId());
				condition.put("value", c.getValue());
				condition.put("type", c.getType());
				condition.put("thing", c.getThing().getId());
				condition.put("device name", c.getThing().getName());
				//inner.put("" +c.getId(), condition);
				inner.append("condition", condition);
			}
			
			for (Action a : auto.getActions()) {
				JSONObject action = new JSONObject();
				action.put("id", a.getId());
				action.put("value", a.getValue());
				action.put("name", a.getName());
				action.put("thing", a.getThing().getId());
				action.put("device name", a.getThing().getName());
				//inner.put("" + a.getId(), action);
				inner.append("action", action);
			}
			json.append("automation", inner);
		}
		return json;
	}
}
