import static org.junit.Assert.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
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
import org.apache.http.client.methods.HttpPost;
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
import org.junit.BeforeClass;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

//https://www.javacodegeeks.com/2012/09/simple-rest-client-in-java.html
@SuppressWarnings("deprecation")
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class TestRestUser extends TestRest {


	
	@BeforeClass
	public static void setup(){
		TestRestUser.httpContext.setAttribute(HttpClientContext.COOKIE_STORE, cookieStore);
	}
	
	@Test
	public void a_loginDefaultUser() throws ClientProtocolException, IOException {		
		login();
	}

	
	@Test 
	public void d_createNewUser() throws ClientProtocolException, IOException{		
		/* create new user */
		HttpPut requestCreate = new HttpPut(URL +":8080/SHP-Web/rest/gui/login");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("email", "asd@asd.de")); //you can as many name value pair as you want in the list.
		nameValuePairs.add(new BasicNameValuePair("password", "qwertzuiop"));
		requestCreate.setEntity( new UrlEncodedFormEntity(nameValuePairs));
		HttpResponse responseCreate = httpClient.execute(requestCreate,httpContext);
		String create = getAsString(responseCreate);
		assertTrue(create.contains("successful"));	
		System.out.println("create");		
	}
	
	@Test
	public void e_loginNewUser() throws ClientProtocolException, IOException{
		/* login as new user */
		HttpGet requestLogin = new HttpGet(URL +":8080/SHP-Web/rest/gui/login/asd@asd.de/qwertzuiop");
		HttpResponse responseLogin = httpClient.execute(requestLogin,httpContext);
		String login = getAsString(responseLogin);
		assertTrue(login.contains("successful"));
		System.out.println("login");		
		
	}
	@Test
	public void f_deleteNewUser() throws ClientProtocolException, IOException{
		/* delete as new user */
		HttpDelete getThings = new HttpDelete(URL +":8080/SHP-Web/rest/gui/login");
		HttpResponse response2 = httpClient.execute(getThings, httpContext);		
		String delete = getAsString(response2);
		assertTrue(delete.contains("successful"));
		System.out.println("delete");	
	}
	
	
	@Test 
	public void g_createNewUserWrongParamter() throws ClientProtocolException, IOException{		
		/* create new user */
		HttpPost requestCreate = new HttpPost(URL +":8080/SHP-Web/rest/gui/login");
		List<BasicNameValuePair> nameValuePairs = new ArrayList<BasicNameValuePair>();		 
		nameValuePairs.add(new BasicNameValuePair("email", "fail")); //you can as many name value pair as you want in the list.
		nameValuePairs.add(new BasicNameValuePair("password", "fail"));
		requestCreate.setEntity( new UrlEncodedFormEntity(nameValuePairs));
		HttpResponse responseCreate = httpClient.execute(requestCreate,httpContext);
		String create = getAsString(responseCreate);
		assertTrue(create.contains("fail"));	
		System.out.println("createWrong");		
	}
	
	@Test
	public void g_loginWrongUser() throws ClientProtocolException, IOException{
		/* login as new user */
		HttpGet requestLogin = new HttpGet(URL +":8080/SHP-Web/rest/gui/login/fail/fail");
		HttpResponse responseLogin = httpClient.execute(requestLogin,httpContext);
		String login = getAsString(responseLogin);
		assertTrue(login.contains("fail"));
		System.out.println("loginWrong");		
		
	}
	
	

}
