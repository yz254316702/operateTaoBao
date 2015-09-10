package com.owenyi.input;

import android.content.Intent;
import android.content.IntentFilter;
import android.inputmethodservice.InputMethodService;
import android.view.View;

public class InputService extends InputMethodService {

	private ReceiveStringService mReceiveStringSerice;
	@Override
	public void onCreate() {
		super.onCreate();
		mReceiveStringSerice = new ReceiveStringService();
		mReceiveStringSerice.setInputService(this);
		IntentFilter inf = new IntentFilter("com.owenyi.input.INPUTSTRING");
		getApplicationContext().registerReceiver(mReceiveStringSerice,inf);
	}
	@Override
	public void onDestroy() {
		super.onDestroy();
		mReceiveStringSerice.setInputService(null);
		getApplicationContext().unregisterReceiver(mReceiveStringSerice);
	}

	public void outPutText(String v) {
		if (getCurrentInputConnection() != null) {
			getCurrentInputConnection().commitText(v, 0);
		}
	}
	
	@Override
	public View onCreateInputView() {
		return null;
	}
}
