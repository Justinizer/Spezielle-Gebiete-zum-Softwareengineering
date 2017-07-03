import static org.junit.Assert.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.CookieStore;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpDelete;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.impl.client.BasicCookieStore;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;
import org.json.JSONArray;
import org.json.JSONObject;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

//https://www.javacodegeeks.com/2012/09/simple-rest-client-in-java.html
@SuppressWarnings("deprecation")
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class TestRest {

	String URL = "http://127.0.0.1";
	
	
	HttpClient httpClient = new DefaultHttpClient();
	CookieStore cookieStore = new BasicCookieStore();
	HttpContext httpContext = new BasicHttpContext();
	

	@Test
	public void aloginDefaultUser() throws ClientProtocolException, IOException {
		
		HttpClient client = HttpClientBuilder.create().build();
		HttpGet request = new HttpGet(URL +":8080/SHP-Web/rest/gui/login/a/a");
		HttpResponse response = client.execute(request);
		String answer = getAsString(response);
		assertTrue(answer.contains("successful"));

	}

	
	@Test 
	public void cloginNewUser() throws ClientProtocolException, IOException{
		httpContext.setAttribute(HttpClientContext.COOKIE_STORE, cookieStore);
		/* create new user */
		HttpPut requestCreate = new HttpPut(URL +":8080/SHP-Web/rest/gui/login");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("email", "asd@asd.de")); //you can as many name value pair as you want in the list.
		nameValuePairs.add(new BasicNameValuePair("password", "qwertzuiop"));
		requestCreate.setEntity( new UrlEncodedFormEntity(nameValuePairs));
		HttpResponse responseCreate = httpClient.execute(requestCreate,httpContext);
		String create = getAsString(responseCreate);
		assertTrue(create.contains("successful"));
		
		/* login as new user */
		HttpGet requestLogin = new HttpGet(URL +":8080/SHP-Web/rest/gui/login/asd@asd.de/qwertzuiop");
		HttpResponse responseLogin = httpClient.execute(requestLogin,httpContext);
		String login = getAsString(responseLogin);
		assertTrue(login.contains("successful"));
		
		/* delete as new user */
		HttpDelete getThings = new HttpDelete(URL +":8080/SHP-Web/rest/gui/login/delete");
		HttpResponse response2 = httpClient.execute(getThings, httpContext);		
		String delete = getAsString(response2);
		assertTrue(delete.contains("successful"));
	}
	
	@Test
	public void testThings() throws ClientProtocolException, IOException{
		httpContext.setAttribute(HttpClientContext.COOKIE_STORE, cookieStore);
		/* login as standard user */
		HttpGet requestCreate = new HttpGet(URL +":8080/SHP-Web/rest/gui/login/a/a");		
		HttpResponse responseLogin = httpClient.execute(requestCreate,httpContext);
		String create = getAsString(responseLogin);
		assertTrue(create.contains("successful"));
		
		/* create thing */
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
		String id = testThing.get("id").toString();
		
		/* get all things */
		HttpGet requestGetThings = new HttpGet(URL +":8080/SHP-Web/rest/gui/thing");
		HttpResponse responseThings = httpClient.execute(requestGetThings,httpContext);
		String thingString = getAsString(responseThings);
		
		/* check if newly created thing is in thingslist */
		JSONArray o = new JSONArray(thingString);
		Iterator i = o.iterator();
		boolean found = false;
		while(i.hasNext()){
			JSONObject thing =  (JSONObject) i.next();
			if(thing.get("name").equals("TEST")){
				found = true;
			}			
		}
		assertTrue(found);
		
		
		/* delete thing */
		HttpDelete requestDeleteThings = new HttpDelete(URL +":8080/SHP-Web/rest/gui/thing/" +id);
		HttpResponse responseDeleteThings = httpClient.execute(requestDeleteThings,httpContext);
		String thingDeleteString = getAsString(responseDeleteThings);
		assertTrue(thingDeleteString.contains("successful"));
		
		
	}

	private String getAsString(HttpResponse response) {
		String line = "";
		try {
			BufferedReader rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
			String buffer = "";
			while ((buffer = rd.readLine()) != null) {
				line += buffer + "\n";
			}
			return line;
		} catch (UnsupportedOperationException | IOException e) {
		}
		return line;
	}

}
