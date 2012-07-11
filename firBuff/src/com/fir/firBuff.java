package com.fir;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class firBuff extends Activity {
	
	private Button goButton;
	private TextView notifView;
	private static native void firBuffTest();
	buffThread buff;
	
	static{
		System.loadLibrary("buffer");
	};
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        buff = new buffThread();
        
        goButton = (Button)this.findViewById(R.id.goButton);
        notifView = (TextView)this.findViewById(R.id.notif);
        
        goButton.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				buff.start();
				
			}
		});
        
    }

    @SuppressWarnings("unused")
	private class buffThread extends Thread {
    	public void run(){
    		firBuffTest();
    	}
    }
}