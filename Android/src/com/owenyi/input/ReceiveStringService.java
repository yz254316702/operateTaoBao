package com.owenyi.input;

import java.util.List;

import com.android.uiautomator.core.UiObject;
import com.android.uiautomator.core.UiObjectNotFoundException;
import com.android.uiautomator.core.UiSelector;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
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
		Log.d(TAG,"onReceive  --- "+arg1.getAction());
		Log.d(TAG,"onReceive  --- "+arg1.getStringExtra(stringKey));
		Log.d(TAG,"onReceive  --- "+arg1.getBooleanExtra(booleanKey,false));
		if (mInputService != null) {
			mInputService.outPutText(arg1.getStringExtra(stringKey));
			if(arg1.getBooleanExtra(booleanKey,false)) {
				startAPP(TaoBaoPackageName);
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
}
