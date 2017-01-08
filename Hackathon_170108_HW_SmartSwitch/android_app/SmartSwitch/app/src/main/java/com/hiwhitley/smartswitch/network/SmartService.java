package com.hiwhitley.smartswitch.network;

import com.hiwhitley.smartswitch.entity.Device;
import com.hiwhitley.smartswitch.entity.ResponseBody;
import com.hiwhitley.smartswitch.entity.User;

import java.util.List;

import retrofit2.Call;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.POST;

/**
 * Created by hiwhitley on 2017/1/5.
 */

public interface SmartService {
    @FormUrlEncoded
    @POST("getDevices")
    Call<List<Device>> getDeviceList(@Field("userID") String userId);

    @FormUrlEncoded
    @POST("getNewOneDevices")
    Call<Device> getNewOneDevices(@Field("deviceID") String deviceID);

    @POST("todoSwitch")
    @FormUrlEncoded
    Call<Device> todoSwitch(@Field("deviceID") String deviceID, @Field("status") int status);

    @POST("updateNickName")
    @FormUrlEncoded
    Call<Device> updateNickName(@Field("deviceID") String deviceID, @Field("nickName") String nickName);

    @POST("saveUserInfo")
    @FormUrlEncoded
    Call<User> saveUserInfo(@Field("tel") String tel, @Field("password") String password);

    @POST("login")
    @FormUrlEncoded
    Call<User> login(@Field("userID") String userID, @Field("password") String password);

    @POST("bindUserAndDev")
    @FormUrlEncoded
    Call<ResponseBody> bindUserAndDev(@Field("userID") String userID, @Field("deviceID") String deviceID);
}
