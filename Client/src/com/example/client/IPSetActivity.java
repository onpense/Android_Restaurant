package com.example.client;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import com.example.socket.MySocketThread;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class IPSetActivity extends Activity implements OnClickListener{
	private Button btn;
	private EditText ed;
	private Handler handler;
	//String str="192.168.7.114";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_ipset);

		
		ed=(EditText) findViewById(R.id.edit_ip);
		ed.setText("192.168.1.11");
	//	String str=ed.getText().toString().trim();
		
		btn=(Button) findViewById(R.id.button1);
		btn.setOnClickListener(this);
		

		handler=new Handler() {
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				
				switch(msg.what) {
				case 0:
					Toast.makeText(IPSetActivity.this, "µÇÂ½³É¹¦", Toast.LENGTH_SHORT).show();
						
				default: break;
				}
				
			}
		};
		
		
	}
	
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
	//	System.out.println("hello world");
	//	new socketThread(str).start();
		String str=ed.getText().toString().trim();
		new MySocketThread(handler,str).start();
	
		Intent intent=new Intent( );
		intent.setClass(IPSetActivity.this, OrderActivity.class);
		startActivity(intent);
		
	}

}
