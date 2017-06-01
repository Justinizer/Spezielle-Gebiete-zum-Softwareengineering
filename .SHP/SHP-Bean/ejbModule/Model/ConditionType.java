package Model;

public enum ConditionType {
	
	greater, smaller, equal, notEqual, greaterThan, smallerThan;
	
	public static  ConditionType getByIndex(int index){
		switch(index){
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
}
