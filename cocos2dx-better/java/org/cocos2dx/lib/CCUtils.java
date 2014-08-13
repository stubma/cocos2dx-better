/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.regex.MatchResult;

import org.apache.http.conn.util.InetAddressUtils;

import android.Manifest.permission;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

public class CCUtils {
	public static final String BOGOMIPS_PATTERN = "BogoMIPS[\\s]*:[\\s]*(\\d+\\.\\d+)[\\s]*\n";
	public static final String MEMTOTAL_PATTERN = "MemTotal[\\s]*:[\\s]*(\\d+)[\\s]*kB\n";
	public static final String MEMFREE_PATTERN = "MemFree[\\s]*:[\\s]*(\\d+)[\\s]*kB\n";

	/**
	 * @return in kiloHertz.
	 * @throws SystemUtilsException
	 */
	public static int getCPUFrequencyMax() throws Exception {
		try {
	        return CCUtils.readSystemFileAsInt("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq") * 1000;
        } catch (Exception e) {
        	return 0;
        }
	}

	private static int readSystemFileAsInt(final String pSystemFile) throws Exception {
		InputStream in = null;
		try {
			final Process process = new ProcessBuilder(new String[] { "/system/bin/cat", pSystemFile }).start();

			in = process.getInputStream();
			final String content = readFully(in);
			return Integer.parseInt(content);
		} catch (final Exception e) {
			throw new Exception(e);
		}
	}

	public static final String readFully(final InputStream pInputStream) throws IOException {
		final StringBuilder sb = new StringBuilder();
		final Scanner sc = new Scanner(pInputStream);
		while (sc.hasNextLine()) {
			sb.append(sc.nextLine());
		}
		return sb.toString();
	}

	public static MatchResult matchSystemFile(final String pSystemFile, final String pPattern, final int pHorizon)
	        throws Exception {
		InputStream in = null;
		try {
			final Process process = new ProcessBuilder(new String[] { "/system/bin/cat", pSystemFile }).start();

			in = process.getInputStream();
			final Scanner scanner = new Scanner(in);

			final boolean matchFound = scanner.findWithinHorizon(pPattern, pHorizon) != null;
			if (matchFound) {
				return scanner.match();
			} else {
				throw new Exception();
			}
		} catch (final IOException e) {
			throw new Exception(e);
		}
	}
	
	static void showConfirmDialog(final String title, final String text, final String positiveButton, final String negativeButton, 
			final long okFunc, final long cancelFunc) {
		nativeRetain(okFunc);
		nativeRetain(cancelFunc);
		
		final Cocos2dxActivity act = (Cocos2dxActivity)Cocos2dxActivity.getContext();
		act.runOnUiThread(new Runnable() {
			public void run() {
				AlertDialog.Builder builder = new Builder(act);
				if(title != null)
					builder.setTitle(title);
				if(text != null)
					builder.setMessage(text);
				if(positiveButton != null) {
					builder.setPositiveButton(positiveButton, new OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							act.runOnGLThread(new Runnable() {
								@Override
								public void run() {
									nativeExecuteCallFunc(okFunc);
									nativeRelease(okFunc);
								}
							});
						}
					});
				}
				if(negativeButton != null) {
					builder.setNegativeButton(negativeButton, new OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							act.runOnGLThread(new Runnable() {
								@Override
								public void run() {
									nativeExecuteCallFunc(cancelFunc);
									nativeRelease(cancelFunc);
								}
							});
						}
					});
				}

				builder.show();
			}
		});
	}
	
	public static void openUrl(String url) {
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setData(Uri.parse(url));
		Cocos2dxActivity.getContext().startActivity(intent);
	}
	
	public static void openAppInStore() {
		String pkg = Cocos2dxActivity.getContext().getPackageName();
		String url = String.format("https://play.google.com/store/apps/details?id=%s", pkg);
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setData(Uri.parse(url));
		Cocos2dxActivity.getContext().startActivity(intent);
	}
	
	public static String getDeviceType() {
		String device = Build.DEVICE;
		if(TextUtils.isEmpty(device))
			device = "Android";
		return device;
	}
	
	public static String getMacAddress() {
		String mac = "";
		Context ctx = Cocos2dxActivity.getContext();
		
		// first, try to get mac from wifi manager
		if(ctx.checkCallingPermission(permission.ACCESS_WIFI_STATE) == PackageManager.PERMISSION_GRANTED) {
			WifiManager wifi = (WifiManager)ctx.getSystemService(Context.WIFI_SERVICE);
			WifiInfo info = wifi.getConnectionInfo();
			mac = info.getMacAddress();
		}
		
		// if failed, try from network interface api
		if(TextUtils.isEmpty(mac)) {
			if (Build.VERSION.SDK_INT >= 9) {
				try {
					NetworkInterface ne = NetworkInterface.getByInetAddress(InetAddress.getByName(getLocalIpAddress()));
					byte[] b = ne.getHardwareAddress();
					mac = byte2Hex(b);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		
		// if failed, use fake
		if(TextUtils.isEmpty(mac)) {
			mac = "00:00:00:00:00:00";
		}
		
		// return
		return mac;
	}
	
	public static String byte2Hex(byte[] b) {
		StringBuilder hs = new StringBuilder(b.length);
		String stmp = "";
		int len = b.length;
		for (int n = 0; n < len; n++) {
			stmp = Integer.toHexString(b[n] & 0xFF);
			if (stmp.length() == 1)
				hs = hs.append("0").append(stmp);
			else
				hs = hs.append(stmp);
		}
		return String.valueOf(hs);
	}
	
	public static String getLocalIpAddress() {
		try {
			String ipv4;
			List<NetworkInterface> nilist = Collections.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface ni : nilist) {
				List<InetAddress> ialist = Collections.list(ni.getInetAddresses());
				for (InetAddress address : ialist) {
					if (!address.isLoopbackAddress() && InetAddressUtils.isIPv4Address(ipv4 = address.getHostAddress())) {
						return ipv4;
					}
				}
			}
		} catch (SocketException ex) {
		}
		
		return null;
	}
		
	private static native void nativeExecuteCallFunc(long func);
	private static native void nativeRetain(long obj);
	private static native void nativeRelease(long obj);
}