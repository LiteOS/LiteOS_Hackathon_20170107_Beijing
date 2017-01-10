package com.example.apple.login;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.DotOptions;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.Marker;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationConfiguration;
import com.baidu.mapapi.map.MyLocationData;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.location.BDNotifyListener;//假如用到位置提醒功能，需要import该类
import com.baidu.location.Poi;
import com.baidu.mapapi.map.OverlayOptions;
import com.baidu.mapapi.map.PolylineOptions;
import com.baidu.mapapi.model.LatLng;
import com.baidu.mapapi.model.inner.GeoPoint;
import com.baidu.mapapi.search.core.SearchResult;
import com.baidu.mapapi.search.geocode.GeoCodeResult;
import com.baidu.mapapi.search.geocode.GeoCoder;
import com.baidu.mapapi.search.geocode.OnGetGeoCoderResultListener;
import com.baidu.mapapi.search.geocode.ReverseGeoCodeOption;
import com.baidu.mapapi.search.geocode.ReverseGeoCodeResult;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.NumberFormat;
import java.util.Random;

public class OnMap extends AppCompatActivity {


    private static final String urlMap = "http://10.1.18.7:5000/querydetail";
    private static final int MAP_CONTENT = 4;


    BaiduMap mBaiduMap;
    MapView myMap;
    public MyLocationListenner myListener = new MyLocationListenner();
    public MyRefreshListenner refreshListenner = new MyRefreshListenner();
    LocationClient mLocClient;
    Marker preMarker;

    GeoCoder geoCoder;

    ImageButton back;
    ImageButton refresh;
    TextView map_deviceType;
    TextView userAddr;
    TextView deviceAddr;
    Button distance;


    String username;
    String info;
    String mac;

    double mac_latitude;
    double mac_longitude;

    double user_latitude;
    double user_longitude;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        SDKInitializer.initialize(getApplicationContext());

        setContentView(R.layout.activity_on_map);

        Init();

        RequestUpdateMacAddress();

        mLocClient.registerLocationListener(myListener);


        refresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                RequestUpdateMacAddress();
            }
        });

        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(OnMap.this, DevicesShow.class);
                startActivity(intent);
                finish();
            }
        });

    }

    private void Init(){
        Intent intent = getIntent();

        Bundle bundle = intent.getExtras();

        username = bundle.getString("name");

        info = bundle.getString("type");

        mac = bundle.getString("macAddr");

        back = (ImageButton) findViewById(R.id.back);

        refresh = (ImageButton) findViewById(R.id.refresh);

        myMap = (MapView) findViewById(R.id.bmapView);

        map_deviceType = (TextView) findViewById(R.id.map_deviceType);

        userAddr = (TextView) findViewById(R.id.userAddr);

        deviceAddr = (TextView) findViewById(R.id.deviceAddr);

        distance = (Button) findViewById(R.id.distance);

        map_deviceType.setText(info);

        mBaiduMap = myMap.getMap();

        mBaiduMap.setMyLocationEnabled(true);

        geoCoder = GeoCoder.newInstance();

        mLocClient = new LocationClient(this);

        LocationClientOption option = new LocationClientOption();

        option.setLocationMode(LocationClientOption.LocationMode.Hight_Accuracy);//设置高精度定位定位模式

        option.setCoorType("bd09ll");//设置百度经纬度坐标系格式

        option.setScanSpan(1000);//设置发起定位请求的间隔时间为1000ms

        option.setIsNeedAddress(true);//反编译获得具体位置，只有网络定位才可以

        mLocClient.setLocOption(option);

        mLocClient.start();

    }


    private void InitMacAddress(){

        if (preMarker != null){
            preMarker.remove();
        }

        // 设置地理编码检索监听者
        geoCoder.setOnGetGeoCodeResultListener(listener);
        //

//        LatLng point = new LatLng(39.963175, 116.400244);
        LatLng point = new LatLng(mac_latitude, mac_longitude);

        geoCoder.reverseGeoCode(new ReverseGeoCodeOption().location(point));

        // add overlay

        BitmapDescriptor bitmap = BitmapDescriptorFactory
                .fromResource(R.mipmap.device);
        OverlayOptions option = new MarkerOptions()
                .position(point)
                .icon(bitmap);

        preMarker = (Marker) mBaiduMap.addOverlay(option);


        // 计算公里数

        //先做坐标转换：
        // toRadians(double angdeg) return angdeg / 180d * PI;
        //角度制转成弧度制：
        double prevLat = Math.toRadians(user_latitude);
        double prevLng = Math.toRadians(user_longitude);

        double nextLat = Math.toRadians(mac_latitude);
        double nextLng = Math.toRadians(mac_longitude);

        double r = 6371;//地球半径m


        //２地距离为：
        double dis = r * distanceRadians(nextLat, nextLng, prevLat, prevLng);

        NumberFormat ddf1=NumberFormat.getNumberInstance() ;
        ddf1.setMaximumFractionDigits(2);
        String s= ddf1.format(dis) ;

        distance.setText(s + "公里");

//        geoCoder.destroy();

    }

    private static double distanceRadians(double lat1, double lng1, double lat2, double lng2) {
        return arcHav(havDistance(lat1, lat2, lng1 - lng2));

    }

    static double arcHav(double x) {
        return 2 * Math.asin(Math.sqrt(x));
    }

    static double havDistance(double lat1, double lat2, double dLng) {
        return hav(lat1 - lat2) + hav(dLng) * Math.cos(lat1) * Math.cos(lat2);
    }

    static double hav(double x) {
        double sinHalf = Math.sin(x * 0.5);
        return sinHalf * sinHalf;
    }


    private void RequestUpdateMacAddress(){
        Thread t = new Thread(new Runnable(){

            @Override
            public void run() {
                HttpURLConnection connection = null;
                try{
                    connection = (HttpURLConnection)((new URL(urlMap.toString())
                            .openConnection()));

                    // set method
                    connection.setRequestMethod("POST");
                    connection.setConnectTimeout(800000); //80s
                    connection.setReadTimeout(800000);//80s

                    DataOutputStream out = new DataOutputStream(
                            connection.getOutputStream());
                    //use post method to post our data
                    out.writeBytes("username=" + username
                            + "&macaddr=" + mac);



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

                    message.what = MAP_CONTENT;
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
                case MAP_CONTENT:
                    String res = message.obj.toString();
                    System.out.println(res);
                    try{
                        JSONObject dataJson = new JSONObject(res);
                        mac_latitude = dataJson.getDouble("latitude");
                        mac_longitude = dataJson.getDouble("longitude");

                        InitMacAddress();

                    } catch (JSONException e){
                        e.printStackTrace();
                    }
                    break;
                default:
                    break;
            }
        }
    };

    OnGetGeoCoderResultListener listener = new OnGetGeoCoderResultListener() {
        // 反地理编码查询结果回调函数
        @Override
        public void onGetReverseGeoCodeResult(ReverseGeoCodeResult result) {
            if (result == null
                    || result.error != SearchResult.ERRORNO.NO_ERROR) {
                // 没有检测到结果
                Toast.makeText(OnMap.this, "抱歉，未能找到结果",
                        Toast.LENGTH_LONG).show();
            }

            deviceAddr.setText("设备位置：" + result.getAddress());
        }

        // 地理编码查询结果回调函数
        @Override
        public void onGetGeoCodeResult(GeoCodeResult result) {
            if (result == null
                    || result.error != SearchResult.ERRORNO.NO_ERROR) {
                // 没有检测到结果
            }
        }
    };


    public class MyLocationListenner implements BDLocationListener {

        @Override
        public void onReceiveLocation(BDLocation location) {

            if (location == null || myMap == null)
                return;


            double latitude = location.getLatitude();  // 获得纬度
            double longitude = location.getLongitude(); // 获得经度
            String currentAdd = location.getAddrStr(); //  获取地址名称

            MapStatusUpdate update = MapStatusUpdateFactory.newLatLng(new LatLng(latitude, longitude));
            // 移动到某经纬度
            mBaiduMap.animateMapStatus(update);


//            //定义Maker坐标点
            LatLng point = new LatLng(latitude, longitude);
//            //构建Marker图标
            BitmapDescriptor bitmap = BitmapDescriptorFactory
                    .fromResource(R.mipmap.greenspot3x);
//            //构建MarkerOption，用于在地图上添加Marker
            OverlayOptions option = new MarkerOptions()
                    .position(point)
                    .icon(bitmap);
//            //在地图上添加Marker，并显示
            mBaiduMap.addOverlay(option);

            userAddr.setText("您当前的位置是：" + currentAdd);

            user_latitude = latitude;
            user_longitude = longitude;

            InitMacAddress();
        }

        public void onReceivePoi(BDLocation poiLocation) {

        }
    }

    public class MyRefreshListenner implements View.OnClickListener{

        @Override
        public void onClick(View v) {
            GetPosition();

        }
    }

    private void GetPosition(){

    }

}
