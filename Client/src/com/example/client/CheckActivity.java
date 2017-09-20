package com.example.client;


import java.util.List;

import com.example.socket.MySocketThread;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class CheckActivity extends Activity implements OnClickListener{


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.check);
		float sum=0;
		for(int i=0;i<FoodActivity.dishdata.size();i++) {
			//sum+=la.get(i).
			int count=FoodActivity.dishdata.get(i).count;
			int price=Integer.parseInt(FoodActivity.dishdata.get(i).price);
			sum+=count*price;
		}
		Button btn=(Button) findViewById(R.id.button2);
		btn.setText(String.valueOf(sum));
		
		Button bt=(Button) findViewById(R.id.button3);
		bt.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		MySocketThread.writeCmd("c");
		Log.e("main","send+c");
	}
}
