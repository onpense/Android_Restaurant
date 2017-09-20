package com.example.client;

import com.example.socket.MySocketThread;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;

public class OrderActivity extends Activity implements OnClickListener{

	private ImageView pic_order;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.order);
		
		pic_order=(ImageView) findViewById(R.id.tv_order);
		pic_order.setOnClickListener(this);
		
		ImageView pic_urge=(ImageView) findViewById(R.id.tv_urge);
		pic_urge.setOnClickListener(this);
		
		ImageView pic_check=(ImageView) findViewById(R.id.tv_check);
		pic_check.setOnClickListener(this);
	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		Intent intent=new Intent();
		switch(v.getId()) {
		case R.id.tv_order:
			
			intent.setClass(this, DishActivity.class);
			startActivity(intent);
			
			break;
		case R.id.tv_urge:
			MySocketThread.writeCmd("r");
			Log.e("main","send+r");
			break;
		
		case R.id.tv_check:
			intent.setClass(this, CheckActivity.class);
			startActivity(intent);
		}
	
	}


}
