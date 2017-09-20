package com.example.client;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

//import com.example.day5_listview.MainActivity;
import com.example.socket.MySocketThread;

import android.app.Activity;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class FoodActivity extends Activity implements OnClickListener{

	static List<Dish> dishdata=new ArrayList<Dish>();
	String str;
	Handler handler;
	DishAdapter da;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.food);
	
		
		handler=new Handler() {
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				Toast.makeText(FoodActivity.this, "菜单传送成功", Toast.LENGTH_SHORT).show();
				jsonToList((String)msg.obj);
				Log.e("main",(String)msg.obj);
				
				ListView lv=(ListView) findViewById(R.id.listView);
				da=new DishAdapter();
				lv.setAdapter(da);
			}
		};
		
		
		Button btn=(Button) findViewById(R.id.button1);
		btn.setOnClickListener(this);
		
		Button bt=(Button) findViewById(R.id.button2);
		bt.setOnClickListener(this);
		
	}
	//点击获取菜单
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch(v.getId()) {
			case R.id.button1:
				MySocketThread.setHandler(handler);
				MySocketThread.writeCmd("m");
				break;
			case R.id.button2:
				HandInMenu();
			}
			
		}
	
	private void HandInMenu() {
		// TODO Auto-generated method stub
//		o{"total number":3,"deskid":8,"carte":[{"id":1,"number":1},{"id":3,"number":2},{"id":2,"number":1}],"orderid":0}
		String cmd="";
//		int dishSpecies=0;
		JSONObject object=new JSONObject();
		try {
			object.put("total number", dishdata.size());
			object.put("deskid", 8);
			JSONArray arr=new JSONArray();
			for(int i=0;i<dishdata.size();i++) {
				JSONObject o=new JSONObject();
				o.put("id", dishdata.get(i).dishid);
				o.put("number", dishdata.get(i).count);		
				arr.put(o);
			}
			object.put("carte", arr);
			object.put("orderid",0);
			cmd="o"+object.toString().trim();
			MySocketThread.writeCmd(cmd);
			
			Log.e("main",cmd);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	public void jsonToList(String msg) {
		
		try {
			JSONObject jsonObject=new JSONObject(msg);
			JSONArray arr=jsonObject.getJSONArray("carte");
			for(int i=0;i<arr.length();i++) {
				Dish dish=new Dish();
				JSONObject object=arr.getJSONObject(i);
				dish.dishid=object.getString("dishid");
				dish.dishname=object.getString("dishname");
				dish.price=object.getString("price");
				dish.storage=object.getString("storage");
				dish.type=object.getString("type");
				dishdata.add(dish);
				Log.e("main",dish.dishname);
			}
			Log.e("main","hello world");
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	class DishAdapter extends BaseAdapter implements OnClickListener {
		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return dishdata.size();
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return 0;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
						
			LayoutInflater inf=LayoutInflater.from(FoodActivity.this);
			View view=inf.inflate(R.layout.fooditem, null);
			
			TextView tv_dishid=(TextView)view.findViewById(R.id.tv_dishid);
			TextView tv_dishname=(TextView) view.findViewById(R.id.tv_dishname);
			TextView tv_price=(TextView) view.findViewById(R.id.tv_price);
//			TextView tv_storage=(TextView) view.findViewById(R.id.tv_storage);
//			TextView tv_type=(TextView) view.findViewById(R.id.tv_type);
			
			Dish dish=dishdata.get(position);

			tv_dishid.setText(dish.dishid);
			tv_dishname.setText(dish.dishname);
			tv_price.setText(dish.price);
//			tv_storage.setText(dish.storage);
//			tv_type.setText(dish.type);
			
			//设置点菜数量	
			ImageView iv_jian=(ImageView) view.findViewById(R.id.iv_jian);
			iv_jian.setOnClickListener(this);
			iv_jian.setTag(position);
			
			ImageView iv_jia=(ImageView) view.findViewById(R.id.iv_jia);
			iv_jia.setOnClickListener(this);
			iv_jia.setTag(position);
			
			Button bt_count=(Button) view.findViewById(R.id.button_count);
			bt_count.setText(String.valueOf(dish.count));
			return view;
		}

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			int index=(Integer) v.getTag();
			Dish dish=dishdata.get(index);
			switch(v.getId()) {
			case R.id.iv_jian:
				dish.count=dish.count-1;break;
			case R.id.iv_jia:
				dish.count=dish.count+1;break;
			}
			notifyDataSetChanged();
		}		
	}

}
class Dish{
	public String dishid;
	public String dishname;
	public String price;
	public String storage;
	public String type;
	public int count=0;
}