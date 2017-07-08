import static org.junit.Assert.*;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;


import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpDelete;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONObject;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class TestRestAutomation extends TestRest {
	static String newID="";
	static String thingID="";
	static String actionID ="";
	static String conditionID ="";
	
	@Test
	public void a_TestCreateAutomationWithoutLogin() throws ClientProtocolException, IOException{						
		assertTrue(createTestAutomation().contains("fail"));		
	}
	
	@Test
	public void b_TestCreateAutomation() throws ClientProtocolException, IOException{	
		
		if(!login()){
			fail();
		}
		String answer = createTestAutomation();
		System.out.println("b: " + answer);
		assertTrue(answer.contains("successful"));			
		JSONObject obj =  new JSONObject(answer);
		newID = obj.get("id").toString();
		assertTrue(newID.length()>0);
		assertTrue(obj.get("name").toString().equals("TEST"));		
	}	
	
	@Test
	public void c_TestCreateAction() throws ClientProtocolException, IOException{	
		thingID =createThing();
		
		HttpPost requestAdd = new HttpPost(URL +":8080/SHP-Web/rest/gui/automation/action");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("name", "TESTACTION")); 
		nameValuePairs.add(new BasicNameValuePair("autoid", newID));		
		nameValuePairs.add(new BasicNameValuePair("thing", thingID));	
		nameValuePairs.add(new BasicNameValuePair("value", "TESTACTION"));	
		requestAdd.setEntity( new UrlEncodedFormEntity(nameValuePairs));		
		HttpResponse responseAdd = httpClient.execute(requestAdd,httpContext);
		String resp =  getAsString(responseAdd);	
		assertTrue(resp.contains("TESTACTION"));	
		
		JSONObject thing = new JSONObject(resp);
		JSONArray actions = thing.getJSONArray("actions");
		actionID = actions.getJSONObject(0).get("id").toString();
	}
	
	@Test
	public void d_TestCreateCondition() throws  ClientProtocolException, IOException{	
		HttpPost requestAdd = new HttpPost(URL +":8080/SHP-Web/rest/gui/automation/condition");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("autoid", newID)); 
		nameValuePairs.add(new BasicNameValuePair("thingid", thingID));		
		nameValuePairs.add(new BasicNameValuePair("type", "1"));	
		nameValuePairs.add(new BasicNameValuePair("value", "10"));	
		requestAdd.setEntity( new UrlEncodedFormEntity(nameValuePairs));	
		HttpResponse responseAdd = httpClient.execute(requestAdd,httpContext);
		String resp =  getAsString(responseAdd);	
		
		JSONObject thing = new JSONObject(resp);
		
		JSONArray cons = thing.getJSONArray("conditions");
		assertTrue(cons.getJSONObject(0).get("value").toString().equals("10"));
		conditionID = cons.getJSONObject(0).get("id").toString();
	}
	
	@Test
	public void e_TestAutomationWorking() throws ClientProtocolException, IOException, InterruptedException{
		HttpPost requestCreate = new HttpPost(URL +":8080/SHP-Web/rest/gui/thing");
		
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("id", thingID)); //you can as many name value pair as you want in the list.
		nameValuePairs.add(new BasicNameValuePair("value", "0"));
		requestCreate.setEntity( new UrlEncodedFormEntity(nameValuePairs));
		
		HttpResponse responseCreate = httpClient.execute(requestCreate,httpContext);
		String thingString = getAsString(responseCreate);
		assertTrue(thingString.contains("successful"));	
		
		Thread.sleep(1250);
		HttpGet get = new HttpGet(URL +":8080/SHP-Web/rest/gui/thing/" + thingID);
		HttpResponse getRest = httpClient.execute(get,httpContext);
		String string = getAsString(getRest);
		System.out.println(string);
		assertTrue(string.contains("TESTACTION"));	
	}
	
	
	@Test
	public void w_TestDeleteCondition() throws ClientProtocolException, IOException{
		HttpDelete del = new HttpDelete(URL +":8080/SHP-Web/rest/gui/automation/condition/" + conditionID);
		HttpResponse responseAdd = httpClient.execute(del,httpContext);
		String resp =  getAsString(responseAdd);
		System.out.println("DELETE CON: " + resp);
		assertTrue(!resp.contains("fail"));
	}
	
	@Test
	public void x_TestDeleteAction()throws ClientProtocolException, IOException{
		HttpDelete del = new HttpDelete(URL +":8080/SHP-Web/rest/gui/automation/action/" + actionID);
		HttpResponse responseAdd = httpClient.execute(del,httpContext);
		String resp =  getAsString(responseAdd);	
		assertTrue(!resp.contains("fail"));
	}

	
	
	@Test
	public void y_TestDeleteAutomation() throws ClientProtocolException, IOException{	
		HttpPost requestAdd = new HttpPost(URL +":8080/SHP-Web/rest/gui/automation/delete");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("automationid", newID)); 
		requestAdd.setEntity( new UrlEncodedFormEntity(nameValuePairs));		
		HttpResponse responseAdd = httpClient.execute(requestAdd,httpContext);
		String resString = getAsString(responseAdd);
		System.out.println(resString);
		assertTrue(resString.contains("succes"));		
	}
	
	
	@Test
	public void z_TestDeleteThing() throws ClientProtocolException, IOException{
		/* delete thing */
		HttpDelete requestDeleteThings = new HttpDelete(URL +":8080/SHP-Web/rest/gui/thing/" +thingID);
		HttpResponse responseDeleteThings = httpClient.execute(requestDeleteThings,httpContext);
		String thingDeleteString = getAsString(responseDeleteThings);
		System.out.println(thingDeleteString);
		assertTrue(!thingDeleteString.contains("fail"));
	}
	
	
	
	private String createTestAutomation() throws ClientProtocolException, IOException{
		HttpPost requestAdd = new HttpPost(URL +":8080/SHP-Web/rest/gui/automation/create");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("name", "TEST")); 
		nameValuePairs.add(new BasicNameValuePair("active", "TRUE"));		
		requestAdd.setEntity( new UrlEncodedFormEntity(nameValuePairs));		
		HttpResponse responseAdd = httpClient.execute(requestAdd,httpContext);
		return getAsString(responseAdd);	
	}
	
	private String createThing() throws ClientProtocolException, IOException{
		HttpPut requestAdd = new HttpPut(URL +":8080/SHP-Web/rest/gui/thing");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("name", "TEST")); 
		nameValuePairs.add(new BasicNameValuePair("mqttTopic", "TEST/TEST"));
		nameValuePairs.add(new BasicNameValuePair("type", "1"));
		nameValuePairs.add(new BasicNameValuePair("unit", "TESTS"));		
		requestAdd.setEntity( new UrlEncodedFormEntity(nameValuePairs));		
		HttpResponse responseAdd = httpClient.execute(requestAdd,httpContext);
		String addString = getAsString(responseAdd);		
		JSONObject testThing = new JSONObject(addString);
		/* check answer */
		
		return testThing.get("id").toString();	
	}
	

}
