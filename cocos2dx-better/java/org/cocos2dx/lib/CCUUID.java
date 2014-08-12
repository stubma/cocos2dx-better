package org.cocos2dx.lib;

import java.util.UUID;

public class CCUUID {
	public static String generate(boolean noHyphen) {
		UUID uuid = UUID.randomUUID();
		String uuidStr = uuid.toString();
		if(noHyphen) {
			uuidStr = uuidStr.replaceAll("-", "");
		}
		return uuidStr;
	}
}
