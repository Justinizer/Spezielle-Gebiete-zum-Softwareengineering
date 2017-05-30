//package Bean;
//
//import java.util.Random;
//
//import javax.annotation.PostConstruct;
//import javax.ejb.DependsOn;
//import javax.ejb.EJB;
//import javax.ejb.Singleton;
//import javax.ejb.Startup;
//import javax.inject.Inject;
//import javax.interceptor.AroundInvoke;
//
//import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
//import org.eclipse.paho.client.mqttv3.MqttCallback;
//import org.eclipse.paho.client.mqttv3.MqttClient;
//import org.eclipse.paho.client.mqttv3.MqttException;
//import org.eclipse.paho.client.mqttv3.MqttMessage;
//
//import Model.SystemConfig;
//import Model.Thing;
//
///**
// * Session Bean implementation class MqttBean
// */
//@Startup
//@Singleton
//public class MqttBean implements MqttCallback{
//	public MqttClient client;
//
//
//
//	public MqttBean() {
//		System.out.println("!!!Hello From MQTT Bean!!!");
//
//		// SystemConfig sc =
//		// em.createNamedQuery(SystemConfig.GET_CONFIG,SystemConfig.class).getSingleResult();
//
////		try {
////			client = new MqttClient("tcp://" + hb.getSystemConfig().getMqttServer(), "SHP" + new Random().nextInt(500000));
////			client.connect();
////			client.setCallback(this);
////			
////			for(Thing t : hb.getAllThings()){
////				client.subscribe(t.getMqttTopic());
////			}
////			
////			System.out.println("!!!!CONNECTED!!!!");
////		} catch (MqttException e) {
////			// TODO Auto-generated catch block
////			System.out.println("!!!!MQTT EXCEPTION!!!!");
////			e.printStackTrace();
////		}
//	}
//	
//	@PostConstruct
//	  public void init() {
//	    
//	  }
//
//	@Override
//	public void connectionLost(Throwable arg0) {
//		// TODO Auto-generated method stub
//		
//	}
//
//	@Override
//	public void deliveryComplete(IMqttDeliveryToken arg0) {
//		// TODO Auto-generated method stub
//		
//	}
//
//	@Override
//	public void messageArrived(String arg0, MqttMessage arg1) throws Exception {
//		// TODO Auto-generated method stub
//		System.out.println(arg0  + " "  + arg1.getPayload());
//		
//	}
//
//}
