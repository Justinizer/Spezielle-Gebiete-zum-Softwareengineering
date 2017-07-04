import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
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
public class TestRestThing extends TestRest {

	static String newID ="";
	
	@Test
	public void a_TestGetThingsWithoutLogin() throws ClientProtocolException, IOException {
		String thingString = getAllThings();
		assertTrue(thingString.contains("fail"));
	}
	
	@Test
	public void b_TestCreateThing() throws ClientProtocolException, IOException{
		httpContext.setAttribute(HttpClientContext.COOKIE_STORE, cookieStore);
		login();
		
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
		assertTrue(testThing.get("name").equals("TEST"));
		assertTrue(testThing.get("mqtttopic").equals("TEST/TEST"));
		assertTrue(testThing.get("type").equals("DigitalActor"));
		assertTrue(testThing.get("unit").equals("TESTS"));
		newID = testThing.get("id").toString();		
	}
	
	
	@Test
	public void c_TestGetAllThings() throws ClientProtocolException, IOException{

		String thingString = getAllThings();
		/* check if newly created thing is in thingslist */
		JSONArray o = new JSONArray(thingString);
		Iterator<?> i = o.iterator();
		boolean found = false;
		while(i.hasNext()){
			JSONObject thing =  (JSONObject) i.next();
			if(thing.get("name").equals("TEST")){
				found = true;
			}			
		}
		assertTrue(found);		
	}
	
	
	@Test
	public void d_TestSetValue() throws ClientProtocolException, IOException{
		HttpPost requestCreate = new HttpPost(URL +":8080/SHP-Web/rest/gui/thing");
		
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("id", newID)); //you can as many name value pair as you want in the list.
		nameValuePairs.add(new BasicNameValuePair("value", "MYFRISTTESTVALUE"));
		requestCreate.setEntity( new UrlEncodedFormEntity(nameValuePairs));
		
		HttpResponse responseCreate = httpClient.execute(requestCreate,httpContext);
		String thingString = getAsString(responseCreate);
		assertTrue(thingString.contains("successful"));	
	}
	
	
	@Test
	public void e_TestGetValue() throws ClientProtocolException, IOException, InterruptedException{
		//give the server some time to receive the mqtt message
		Thread.sleep(250);
		HttpGet requestCreate = new HttpGet(URL +":8080/SHP-Web/rest/gui/thing/" + newID);
		HttpResponse responseCreate = httpClient.execute(requestCreate,httpContext);
		String thingString = getAsString(responseCreate);
		System.out.println(thingString);
		assertTrue(thingString.contains("MYFRISTTESTVALUE"));	
	}
	
	
	@Test
	public void f_TestDeleteThing() throws ClientProtocolException, IOException{
		/* delete thing */
		HttpDelete requestDeleteThings = new HttpDelete(URL +":8080/SHP-Web/rest/gui/thing/" +newID);
		HttpResponse responseDeleteThings = httpClient.execute(requestDeleteThings,httpContext);
		String thingDeleteString = getAsString(responseDeleteThings);
		assertTrue(thingDeleteString.contains("successful"));
		System.out.println("c");
	}
	
	
	private String getAllThings() throws ClientProtocolException, IOException{
		/* get all things */
		HttpGet requestGetThings = new HttpGet(URL +":8080/SHP-Web/rest/gui/thing");
		HttpResponse responseThings = httpClient.execute(requestGetThings,httpContext);
		String thingString = getAsString(responseThings);
		return thingString;
	}
	
	
	
}
