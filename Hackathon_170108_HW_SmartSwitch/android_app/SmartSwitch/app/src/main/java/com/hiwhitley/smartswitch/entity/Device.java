package com.hiwhitley.smartswitch.entity;

import java.io.Serializable;

/**
 * Created by hiwhitley on 17-1-5.
 */

public class Device implements Serializable{
    private static final long serialVersionUID = 1L;

    /**
     * deviceID : 1
     * pm : 2.5
     * humidity : 50
     * temperature : 37
     * status : 1
     * nickName : haiweiMate8
     */

    private String deviceID;
    private double pm;
    private double humidity;
    private double temperature;
    private int status;
    private String nickName;

    public String getDeviceID() {
        return deviceID;
    }

    public void setDeviceID(String deviceID) {
        this.deviceID = deviceID;
    }

    public double getPm() {
        return pm;
    }

    public void setPm(double pm) {
        this.pm = pm;
    }

    public double getHumidity() {
        return humidity;
    }

    public void setHumidity(double humidity) {
        this.humidity = humidity;
    }

    public double getTemperature() {
        return temperature;
    }

    public void setTemperature(double temperature) {
        this.temperature = temperature;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }

    @Override
    public String toString() {
        return "Device{" +
                "deviceID='" + deviceID + '\'' +
                ", pm=" + pm +
                ", humidity=" + humidity +
                ", temperature=" + temperature +
                ", status=" + status +
                ", nickName='" + nickName + '\'' +
                '}';
    }
}
