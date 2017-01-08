package com.example.apple.login;

/**
 * Created by apple on 2017/1/6.
 */
public class DeviceInfo {

    private String deviceType;
    private String macAdd;

    public DeviceInfo(String a, String b){
        deviceType = a;
        macAdd = b;
    }

    public String getDeviceType(){
        return deviceType;
    }

    public String getMacAdd(){
        return macAdd;
    }
}
