package com.example.tomydear;

import java.util.Calendar;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.Button;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Calendar startTime = Calendar.getInstance();
        startTime.set(2013, 3 - 1, 3, 15, 30);
        Calendar endTime = Calendar.getInstance();
        endTime.set(2013, 3 - 1, 3, 21, 00);
        Calendar currTime = Calendar.getInstance();
        if (currTime.before(endTime) && currTime.after(startTime))
        {
        	setContentView(R.layout.activity_main);
        	
        	Button yesButton = (Button)findViewById(R.id.button1);
        	YesClickListener yesClick = new YesClickListener();
        	yesClick.setActivity(this);
        	yesButton.setOnClickListener(yesClick);
        	
        	Button noButton = (Button)findViewById(R.id.button2);
        	NoClickListener noClick = new NoClickListener();
        	noClick.setActivity(this);
        	noButton.setOnClickListener(noClick);
        }
        else
        {
        	setContentView(R.layout.activity_main_wrong_time);
        }
        new NetThread().start();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
