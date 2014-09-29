package com.example.tomydear;

import android.app.Activity;
import android.media.MediaPlayer;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class YesClickListener implements OnClickListener {

	private Activity mAct;
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		Button noButton = (Button)mAct.findViewById(R.id.button2);
		TextView text = (TextView)mAct.findViewById(R.id.textView1);
		ImageView heart = (ImageView)mAct.findViewById(R.id.imageView1);
		MediaPlayer sound = MediaPlayer.create(mAct, R.raw.back);
		sound.setLooping(true);
		sound.start();
		arg0.setClickable(false);
		noButton.setClickable(false);
		text.setText(R.string.step_3);
		heart.setVisibility(1);
		
	}

	public void setActivity(Activity act) {
		// TODO Auto-generated method stub
		mAct = act;
	}

}
