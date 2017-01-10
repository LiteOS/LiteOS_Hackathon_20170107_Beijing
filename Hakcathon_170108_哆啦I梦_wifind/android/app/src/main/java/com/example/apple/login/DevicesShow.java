package com.example.apple.login;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.baidu.mapapi.SDKInitializer;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class DevicesShow extends AppCompatActivity {

    private static final String urlDevice = "http://10.1.18.7:5000/querydevice";
    private static final String urlAddDevice = "http://10.1.18.7:5000/adddevice";
    private static final String urlDeleteDevice = "http://10.1.18.7:5000/deletedevice";

    private static final int DEVICE_CONTENT = 1;
    private static final int ADDDEVICE = 2;
    private static final int DELETEDEVICE = 3;
    private static final int REFRESH_COMPLETE = 0X110;
    private static int hasAdded;

    ListView deviceList;
    ImageButton addDevice;
    TextView usershow;
    TextView deviceType;
    TextView macaddr_show;
    Button logout;
    private SwipeRefreshLayout mSwipeLayout;

    String username;
    String devices;
    String mymac;
    String info;

    private List<DeviceInfo> data = new ArrayList<>();
    private List<Map<String, Object>> listItems = new ArrayList<>();
    private SimpleAdapter simpleAdapter;

    @Override

    @SuppressLint("InlinedApi")
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_devices_show);

        InitShow();


        mSwipeLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                handler.sendEmptyMessageDelayed(REFRESH_COMPLETE, 1000);
            }
        });

        sendRequestWithHttpURLConnection();

        deviceList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                DeviceInfo temp = data.get(position);

                Intent intent = new Intent(DevicesShow.this, OnMap.class);
                Bundle bundle = new Bundle();
                bundle.putString("name", username);
                bundle.putString("type", temp.getDeviceType());
                bundle.putString("macAddr", temp.getMacAdd());
                intent.putExtras(bundle);

                startActivity(intent);

            }
        });

        deviceList.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {

                DeviceInfo temp = data.get(position);

                SendDeleteDevice(temp.getMacAdd());

                if (temp.getMacAdd().equals(mymac)){
                    hasAdded = 0;
                    addDevice.setImageResource(R.mipmap.orangeadddevice3x);
                }

                data.remove(position);
                listItems.remove(position);
                simpleAdapter.notifyDataSetChanged();

                return true;
            }
        });

        addDevice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (hasAdded == 1) {
                    return;
                }
                hasAdded = 1;
                addDevice.setImageResource(R.mipmap.graybutton3x);
                AddDevice();
            }
        });

        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(DevicesShow.this, login.class);
                startActivity(intent);
                finish();
            }
        });

    }

    public void InitShow(){
        deviceList = (ListView) findViewById(R.id.deviceList);
        addDevice = (ImageButton) findViewById(R.id.adddevice);
        usershow = (TextView) findViewById(R.id.usershow);
        deviceType = (TextView) findViewById(R.id.deviceType);
        macaddr_show = (TextView) findViewById(R.id.macaddr_show);
        logout = (Button) findViewById(R.id.logout);
        mSwipeLayout = (SwipeRefreshLayout) findViewById(R.id.id_swipe_ly);

        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();

        username = bundle.getString("username");
        usershow.setText(username);

        hasAdded = 0;

        // mac address
        Context context = getApplicationContext();
        WifiManager wifiManager= (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo=wifiManager.getConnectionInfo();
        mymac = wifiInfo.getMacAddress();
        mymac = mymac.toUpperCase();
        macaddr_show.setText(mymac);

        // device type info
        info = Build.MODEL;
        deviceType.setText(username + "的" + info);

    }


    public void updateDevice(){
        listItems.clear();
        for (DeviceInfo c : data){
            Map<String, Object> listItem = new LinkedHashMap<>();
            listItem.put("deviceType", c.getDeviceType());
            listItem.put("macaddr", c.getMacAdd());
            listItems.add(listItem);
        }
        simpleAdapter = new SimpleAdapter(this, listItems, R.layout.deviceitem,
                new String[] {"deviceType", "macaddr"}, new int[] {R.id.item_deviceType, R.id.item_deviceMac});
        deviceList.setAdapter(simpleAdapter);

        simpleAdapter.notifyDataSetChanged();
    }


    private void AddDevice(){
        Thread t = new Thread(new Runnable(){

            @Override
            public void run() {
                HttpURLConnection connection = null;
                try{
                    connection = (HttpURLConnection)((new URL(urlAddDevice.toString())
                            .openConnection()));

                    // set method
                    connection.setRequestMethod("POST");
                    connection.setConnectTimeout(800000); //80s
                    connection.setReadTimeout(800000);//80s

                    DataOutputStream out = new DataOutputStream(
                            connection.getOutputStream());
                    //use post method to post our data
                    out.writeBytes("username=" + username
                            + "&name=" + username + "'s" + info
                            + "&macaddr=" + mymac
                            + "&type=" + info);


                    int code = connection.getResponseCode();

                    //get response data
                    InputStream in = connection.getInputStream();
                    BufferedReader reader = new BufferedReader(
                            new InputStreamReader(in));
                    StringBuilder response = new StringBuilder();

                    String line;
                    while((line = reader.readLine())!=null){
                        response.append(line);
                    }

                    Message message = new Message();

                    message.what = ADDDEVICE;
                    message.obj = response.toString();

                    handler.sendMessage(message);

                } catch(Exception e){
                    e.printStackTrace();
                } finally{
                    if(connection != null){
                        connection.disconnect();
                        handler.removeCallbacks(this);
                    }
                }
            }
        });

        t.start();
    }

    private void sendRequestWithHttpURLConnection(){
        Thread t = new Thread(new Runnable(){
            @Override
            public void run(){
                HttpURLConnection connection = null;
                try{
                    connection = (HttpURLConnection)((new URL(urlDevice.toString())
                            .openConnection()));

                    // set method
                    connection.setRequestMethod("POST");
                    connection.setConnectTimeout(800000); //80s
                    connection.setReadTimeout(800000);//80s

                    DataOutputStream out = new DataOutputStream(
                            connection.getOutputStream());
                    //use post method to post our data
                    out.writeBytes("username=" + username);


                    int code = connection.getResponseCode();

                    //get response data
                    InputStream in = connection.getInputStream();
                    BufferedReader reader = new BufferedReader(
                            new InputStreamReader(in));
                    StringBuilder response = new StringBuilder();

                    String line;
                    while((line = reader.readLine())!=null){
                        response.append(line);
                    }

                    Message message = new Message();

                    message.what = DEVICE_CONTENT;
                    message.obj = response.toString();

                    handler.sendMessage(message);

                } catch(Exception e){
                    e.printStackTrace();
                } finally{
                    if(connection != null){
                        connection.disconnect();
                        handler.removeCallbacks(this);
                    }
                }
            }
        });
        t.start();
    }

    private Handler handler = new Handler(){
        public void handleMessage(Message message){
            switch(message.what){
                case DEVICE_CONTENT:
                    devices = message.obj.toString();

                    if(devices.equals("")){
                        break;
                    }

                    try{

                        data.clear();

                        JSONObject dataJson = new JSONObject(devices);

                        JSONArray arrayJson = dataJson.getJSONArray("devices");

                        for(int i=0;i<arrayJson.length();i++) {
                            JSONObject tempJson = arrayJson.optJSONObject(i);
                            String deviceType = username + "的" + tempJson.getString("type");
                            String macaddr = tempJson.getString("macaddr");

                            DeviceInfo temp = new DeviceInfo(deviceType, macaddr);
//                            System.out.println("handler: " + temp.getDeviceType());
//                            System.out.println("handler: " + temp.getMacAdd());
                            if (macaddr.equals(mymac)){
                                hasAdded = 1;
                            }
                            data.add(temp);
                        }

                        if (hasAdded == 1){
                            addDevice.setImageResource(R.mipmap.graybutton3x);
                        }
                        else {
                            addDevice.setImageResource(R.mipmap.orangeadddevice3x);
                        }

                        updateDevice();

                    } catch (JSONException e) {
                        System.out.println("Something wrong...");
                        e.printStackTrace();
                    }
                    break;
                case ADDDEVICE:
                    System.out.println(message.obj.toString());
                    try{
                        JSONObject dataJson = new JSONObject(message.obj.toString());
                        String res = dataJson.getString("res");
                        if(!res.equals("00000")){
                            Toast.makeText(DevicesShow.this, "添加设备失败，请稍后重试...", Toast.LENGTH_SHORT).show();
                        }
                        else {
                            data.add(new DeviceInfo(username + "的" + info, mymac));
                            updateDevice();
                            addDevice.setImageResource(R.mipmap.graybutton3x);
                        }

                    } catch (JSONException e){
                    e.printStackTrace();
                }
                    break;
                case DELETEDEVICE:
                    System.out.println(message.obj.toString());
                    try{
                        JSONObject dataJson = new JSONObject(message.obj.toString());
                        String res = dataJson.getString("res");
                        if(!res.equals("00000")){
                            Toast.makeText(DevicesShow.this, "删除设备失败，请稍后重试...", Toast.LENGTH_SHORT).show();
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                    break;
                case REFRESH_COMPLETE:
                    sendRequestWithHttpURLConnection();
                    mSwipeLayout.setRefreshing(false);
                    break;
                default:
                    break;
            }
        }
    };

    private void SendDeleteDevice(final String macAddr){
        Thread t = new Thread(new Runnable(){

            @Override
            public void run() {
                HttpURLConnection connection = null;
                try{
                    connection = (HttpURLConnection)((new URL(urlDeleteDevice.toString())
                            .openConnection()));

                    // set method
                    connection.setRequestMethod("POST");
                    connection.setConnectTimeout(800000); //80s
                    connection.setReadTimeout(800000);//80s

                    DataOutputStream out = new DataOutputStream(
                            connection.getOutputStream());
                    //use post method to post our data
                    out.writeBytes("username=" + username
                            + "&macaddr=" + macAddr);


                    int code = connection.getResponseCode();

                    //get response data
                    InputStream in = connection.getInputStream();
                    BufferedReader reader = new BufferedReader(
                            new InputStreamReader(in));
                    StringBuilder response = new StringBuilder();

                    String line;
                    while((line = reader.readLine())!=null){
                        response.append(line);
                    }

                    Message message = new Message();

                    message.what = DELETEDEVICE;
                    message.obj = response.toString();

                    handler.sendMessage(message);

                } catch(Exception e){
                    e.printStackTrace();
                } finally{
                    if(connection != null){
                        connection.disconnect();
                        handler.removeCallbacks(this);
                    }
                }
            }
        });

        t.start();
    }
}
