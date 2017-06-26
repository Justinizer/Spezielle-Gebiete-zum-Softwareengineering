package Model;

import java.util.ArrayList;

public enum ThingType {
	Sensor,
	DigitalActor,
	AnalogActor,
	CameraSensor;
	
	public static ThingType getByIndex(int index) {
		switch (index) {
		case 0:
			return ThingType.Sensor;
		case 1:
			return ThingType.DigitalActor;
		case 2:
			return ThingType.AnalogActor;
		case 3:
			return ThingType.CameraSensor;
		
		}
		return null;
	}

	public static ThingType[] getAll() {
		ArrayList<ThingType> all = new ArrayList<>();
		all.add(ThingType.Sensor);
		all.add(ThingType.DigitalActor);
		all.add(ThingType.AnalogActor);
		all.add(ThingType.CameraSensor);
		
		ThingType[] temp = new ThingType[all.size()];
		return  all.toArray(temp);
	}

	public static String getAsText(ThingType c) {
		switch (c) {
		case Sensor:
			return "Sensor";
		case DigitalActor:
			return "Digitaler Aktor";
		case AnalogActor:
			return "Analoger Aktor";
		case CameraSensor:
			return "Kamera Sensor";
		default:
			return "";
		}
	}
	
	public static int getAsInt(ThingType c) {
		switch (c) {
		case Sensor:
			return 0;
		case DigitalActor:
			return 1;
		case AnalogActor:
			return 2;
		case CameraSensor:
			return 3;
		
		default:
			return -1;
		}
	}
}
