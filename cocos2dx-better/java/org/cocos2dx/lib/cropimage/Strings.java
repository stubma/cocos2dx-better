package org.cocos2dx.lib.cropimage;

import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

public class Strings {
	private static String sLan;
	private static Map<String, Map<String, String>> sLanStrMap;
	
	static {
		sLanStrMap = new HashMap<String, Map<String,String>>();

		// en and zh
		Map<String, String> en = new HashMap<String, String>();
		Map<String, String> zh = new HashMap<String, String>();
		sLanStrMap.put("en", en);
		sLanStrMap.put("zh", zh);
		
		// en strings
		en.put("crop_save_text", "Save");
		en.put("crop_discard_text", "Discard");
		en.put("runningFaceDetection", "Please wait\u2026");
		en.put("wallpaper", "Setting wallpaper, please wait\u2026");
		en.put("savingImage", "Saving picture\u2026");
		en.put("multiface_crop_help", "Tap a face to begin.");
		
		// zh strings
		zh.put("crop_save_text", "保存");
		zh.put("crop_discard_text", "放弃");
		zh.put("runningFaceDetection", "请等待\u2026");
		zh.put("wallpaper", "正在设置壁纸, 请等待\u2026");
		zh.put("savingImage", "正在保存图片\u2026");
		zh.put("multiface_crop_help", "点击脸部开始.");
	}
	
	static String L(String key) {
		if(sLan == null) {
			sLan = Locale.getDefault().getLanguage();
		}
		
		Map<String, String> smap = sLanStrMap.get(sLan);
		if(smap == null) {
			smap = sLanStrMap.get("en");
		}
		
		if(smap.containsKey(key)) {
			return smap.get(key);
		} else {
			return "!" + key + "!";
		}
	}
}
