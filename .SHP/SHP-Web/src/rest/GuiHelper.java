package rest;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.List;

import javax.ws.rs.core.Response;

import org.json.JSONArray;
import org.json.JSONObject;

import Model.Action;
import Model.Automation;
import Model.Condition;
import Model.ConditionType;
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
	
	/**
	 * get a simple status: success json object
	 * @return
	 */
	public JSONObject getSuccess(){
		return success;
	}
	
	/**
	 * get a simple status: fail json object
	 * @return
	 */
	public JSONObject getFail(){
		return fail;
	}
	
	/**
	 * get a json object will all the information about a given automation
	 * @param auto
	 * @return
	 */
	public JSONObject getOneAutomation(Automation auto){
		JSONObject jsonAuto = new JSONObject();
		jsonAuto.put("name", auto.getName());
		jsonAuto.put("id", auto.getId());
		jsonAuto.put("active", auto.isActive());
		JSONArray conditions = new JSONArray();
		for (Condition c : auto.getConditions()) {
			JSONObject condition = new JSONObject();
			condition.put("id", c.getId());
			condition.put("value", c.getValue());
			condition.put("type", ConditionType.getAsInt(c.getType()));
			condition.put("thing", c.getThing().getId());
			condition.put("devicename", c.getThing().getName());
			conditions.put(condition);
		}
		jsonAuto.put("conditions", conditions);

		JSONArray actions = new JSONArray();
		for (Action a : auto.getActions()) {
			JSONObject action = new JSONObject();
			action.put("id", a.getId());
			action.put("value", a.getValue());
			action.put("name", a.getName());
			action.put("thing", a.getThing().getId());
			action.put("devicename", a.getThing().getName());
			actions.put(action);
		}
		jsonAuto.put("actions", actions);
		return jsonAuto;
	}
	
	/**
	 * list all automations with all information about it
	 * @param autos the automations
	 * @return
	 */
	public JSONArray listAutomations(List<Automation>autos){
		JSONArray json = new JSONArray();
		for (Automation auto : autos) {
			json.put( getOneAutomation(auto));
		}
		return json;
	}
	
	/**
	 * get a json object wist all the information about a given thing
	 * @param t
	 * @return
	 */
	public JSONObject thingToJson(Thing t){
		JSONObject inner = new JSONObject();
		inner.put("id", t.getId());
		inner.put("type", t.getType());
		inner.put("name", t.getName());
		inner.put("unit", t.getUnit());
		inner.put("mqtttopic", t.getMqttTopic());
		inner.put("currentValue", getCurrentValue(t));
		return inner;
	}
	
	/**
	 * get a image from a thing
	 * @param t the thing
	 * @return response can be passed the the client without checking.
	 */
	public Response getImageFromID(Thing t){
		try {
			SensorData s = t.getLastSensorData();
			if( s == null){
				return Response.noContent().build();
			}
			String path = s.getValue();
			if(path == null){
				return Response.noContent().build();
			}
			System.out.println("pic path is " + path );
			File file = new File(path);
			if(!file.exists()){
				return Response.noContent().build();
			}			
			return Response.ok(new FileInputStream(file)).build();
		} catch (FileNotFoundException e) {
			
		}
		return Response.noContent().build();
	}
}
