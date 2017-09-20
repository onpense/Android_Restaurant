package com.example.socket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import org.json.JSONException;
import org.json.JSONObject;

import android.os.Handler;
import android.os.Message;

public class MySocketThread extends Thread {

	private Socket client;
	public static Handler handler;
	private String strIP;
	private static OutputStream os;
	public MySocketThread() {
		
	}
	public MySocketThread(Handler handler,String strIp) {
		this.handler=handler;
		this.strIP=strIp;
	}
	public static void setHandler(Handler h) {
		handler=h;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		super.run();
		try {
			client=new Socket(strIP,10000);
			new ReceiveThread().start();
			os=client.getOutputStream();
			String cmd="";
			JSONObject jsonObject=new JSONObject();
			jsonObject.put("uid", 321);
			jsonObject.put("deskid", 8);
			jsonObject.put("uname", "abcUser");
			
			cmd="t"+jsonObject.toString();
			os.write(cmd.getBytes());
		}catch(IOException e) {
			e.printStackTrace();
		}catch(JSONException e) {
			e.printStackTrace();
		}
	}
	
	
	public static void writeCmd(String cmd) {
		try {
			os.write(cmd.getBytes());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	class ReceiveThread extends Thread{
		String content="";
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			super.run();
			InputStream is;
			while(true) {
				try {
					is=client.getInputStream();
					byte[] buffer=new byte[4096];
					is.read(buffer);
					content=new String(buffer,"GB2312").trim();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				if(content.length()>0) {
					Message m=new Message();
					m.obj=content;
					m.what=0;
					handler.sendMessage(m);
				}
				
			}
		}
	}
	
	
	
	
}
