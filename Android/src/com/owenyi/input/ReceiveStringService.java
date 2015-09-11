package com.owenyi.input;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

public class ReceiveStringService extends BroadcastReceiver {
	InputService mInputService;
	private final String TAG = "Input";
	private final static String TaoBaoPackageName = "com.taobao.taobao";
	
	private final String stringKey = "string";
	private final String booleanKey = "boolean";

	public void setInputService(InputService v) {
		mInputService = v;
	}

	@Override
	public void onReceive(Context arg0, Intent arg1) {
		// TODO Auto-generated method stub
		String stringV = arg1.getStringExtra(stringKey);
		boolean boolV = arg1.getBooleanExtra(booleanKey,false);
		Log.d(TAG,"onReceive  --- "+arg1.getAction());
		Log.d(TAG,"onReceive  --- "+stringV);
		Log.d(TAG,"onReceive  --- "+boolV);
		if (mInputService != null) {
			
			if(boolV) {
				if(stringV.equalsIgnoreCase("startapp")) {
					startAPP(TaoBaoPackageName);
				}
				else if(stringV.equalsIgnoreCase("getappinfo")){
					getAppInfo();
				}
			}
			else {
				mInputService.outPutText(arg1.getStringExtra(stringKey));
			}
		}
	}
	
	public void startAPP(String appPackageName){
	    try{
	        Intent intent = mInputService.getApplicationContext().getPackageManager().getLaunchIntentForPackage(appPackageName);
	        mInputService.startActivity(intent);
	        Thread.sleep(2000);
	    }catch(Exception e){
	    	Log.d(TAG,"onReceive  --- "+e.toString());
	        Toast.makeText( mInputService.getApplicationContext(), "没有安装", Toast.LENGTH_LONG).show();
	    }
	}
	private void write2File(String path ,String data) {
		File urlFile = new File(path);
		if(null == data || data.isEmpty() || data.length() == 0) {
			Log.i("LOG-GETBT","null == data || data.isEmpty() || data.length() == 0");
			return ;
		}
		BufferedWriter out = null;
		if (urlFile.exists()) {
			if (urlFile.isDirectory()) {
				Log.i("LOG-GETBT","urlFile.isDirectory()");
				return;
			}
		} else {
			try {
				urlFile.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		try {
			out = new BufferedWriter(new OutputStreamWriter(
					new FileOutputStream(urlFile, false)));
			out.write(data);
			out.write("\n");
			out.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public int getAppInfo() {
		PackageManager pm = mInputService.getPackageManager();
		int versionCode ;
		String VersionName;
		try {
			PackageInfo info = pm.getPackageInfo(TaoBaoPackageName, 0);
			versionCode = info.versionCode;
			VersionName = info.versionName;
			String data = TaoBaoPackageName + " " + versionCode + " "+ VersionName;

			info = pm.getPackageInfo(mInputService.getPackageName(), 0);
			versionCode = info.versionCode;
			VersionName = info.versionName;
			data +=("\n"+mInputService.getPackageName()+ " " + versionCode + " "+ VersionName);
			String fileP = mInputService.getCacheDir().toString() +"/pckInfo.txt";
			write2File(fileP,data);
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return 1;
	}
}
