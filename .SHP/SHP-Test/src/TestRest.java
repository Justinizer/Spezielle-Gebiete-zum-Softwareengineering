

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.CookieStore;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.BasicCookieStore;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

public abstract class TestRest {
	String URL = "http://127.0.0.1";
	
	public TestRest(){
		
	}
	
	protected static HttpClient httpClient = new DefaultHttpClient();
	protected static CookieStore cookieStore = new BasicCookieStore();
	protected static HttpContext httpContext = new BasicHttpContext();
	

	protected boolean login() throws ClientProtocolException, IOException{
		/* login as standard user */
		HttpGet requestCreate = new HttpGet(URL +":8080/SHP-Web/rest/gui/login/a/a");		
		HttpResponse responseLogin = httpClient.execute(requestCreate,httpContext);
		String create = getAsString(responseLogin);
		return create.contains("successful");
	}
	
	protected String getAsString(HttpResponse response) {
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
	
	protected void reset(){
		httpClient = new DefaultHttpClient();
		httpContext = new BasicHttpContext();
		cookieStore = new BasicCookieStore();
	}
}
