package com.example.client;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.widget.ImageView;

public class DishActivity extends Activity implements OnClickListener{

	
	private ImageView principal;  //Ö÷Ê³
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dish);
		
		principal=(ImageView) findViewById(R.id.iv_principal);
		principal.setOnClickListener(this);
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		
		Intent intent=new Intent();
		intent.setClass(this, FoodActivity.class);
		startActivity(intent);
	}

	
}
