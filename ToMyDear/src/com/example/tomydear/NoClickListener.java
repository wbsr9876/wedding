package com.example.tomydear;

import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;

public class NoClickListener implements OnClickListener {

	private Activity mAct;
	@Override
	public void onClick(View arg0) {
		TextView text = (TextView)(mAct.findViewById(R.id.textView1));
		text.setText(R.string.step_2);
		// TODO Auto-generated method stub

	}

	public void setActivity(Activity act) {
		// TODO Auto-generated method stub
		mAct = act;
	}

}
