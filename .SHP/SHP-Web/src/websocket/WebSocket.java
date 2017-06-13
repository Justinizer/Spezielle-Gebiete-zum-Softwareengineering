package websocket;

import java.io.IOException;
import java.io.Serializable;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

import javax.ejb.EJB;

import javax.websocket.EncodeException;
import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

import Bean.MqttBean;
import Interface.MqttBeanRemote;

@ServerEndpoint("/websocket")
public class WebSocket implements Serializable {
	
	public static final Set<Session> clientSessions = Collections.synchronizedSet(new HashSet<Session>());

	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	@EJB
	MqttBeanRemote bh;
	
	/**
	 * Add client session to the Set
	 * 
	 * @param aClientSession
	 */
	@OnOpen
	public void onOpen(Session aClientSession) {
		System.out.println("NEW WS!");
		clientSessions.add(aClientSession);
		MqttBean.clientSessions.add(aClientSession);
		//bh.testi();
	}
	
	/**
	 * Send Text back
	 * 
	 * @param message
	 * @param client
	 * @throws IOException
	 * @throws EncodeException
	 */
	@OnMessage
	public void onMessage(String message, Session client) throws IOException, EncodeException {
		
			
	}
	
	/**
	 * Remove client session from the Set
	 * 
	 * @param aClientSession
	 */
	@OnClose
	public void onClose(Session aClientSession) {
		clientSessions.remove(aClientSession);
	}

	
}
