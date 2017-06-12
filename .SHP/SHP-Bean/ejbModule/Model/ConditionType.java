package Model;

import java.util.ArrayList;

public enum ConditionType {

	greater, smaller, equal, notEqual, greaterThan, smallerThan;

	public static ConditionType getByIndex(int index) {
		switch (index) {
		case 0:
			return ConditionType.greater;
		case 1:
			return ConditionType.smaller;
		case 2:
			return ConditionType.equal;
		case 3:
			return ConditionType.notEqual;
		case 4:
			return ConditionType.greaterThan;
		case 5:
			return ConditionType.smallerThan;
		}
		return null;
	}

	public static ConditionType[] getAll() {
		ArrayList<ConditionType> all = new ArrayList<>();
		all.add(ConditionType.greater);
		all.add(ConditionType.smaller);
		all.add(ConditionType.equal);
		all.add(ConditionType.notEqual);
		all.add(ConditionType.greaterThan);
		all.add(ConditionType.smallerThan);
		ConditionType[] temp = new ConditionType[all.size()];
		return  all.toArray(temp);
	}

	public static String getAsText(ConditionType c) {
		switch (c) {
		case greater:
			return "größer";
		case smaller:
			return "kleiner";
		case equal:
			return "gleich";
		case notEqual:
			return "ungleich";
		case greaterThan:
			return "größer als";
		case smallerThan:
			return "kleiner als";
		default:
			return "";
		}
	}
	
	public static int getAsInt(ConditionType c) {
		switch (c) {
		case greater:
			return 0;
		case smaller:
			return 1;
		case equal:
			return 2;
		case notEqual:
			return 3;
		case greaterThan:
			return 4;
		case smallerThan:
			return 5;
		default:
			return -1;
		}
	}
}
