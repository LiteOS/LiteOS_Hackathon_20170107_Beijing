package com.hiwhitley.smartswitch.network;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * Created by hiwhitley on 2017/1/5.
 */

public class RetrofitFactory {

    public static final String BASE_URL = "http://192.168.0.101:3000/";

    private static Retrofit retrofit;
    private volatile static SmartService mSmartService;

    private RetrofitFactory() {

    }

    private static <T> T configRetrofit(Class<T> clazz, String URL) {
        retrofit = new Retrofit.Builder()
                .baseUrl(URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();
        return retrofit.create(clazz);
    }

    public static SmartService getSmartService() {
        if (mSmartService == null) {
            synchronized (RetrofitFactory.class) {
                if (mSmartService == null) {
                    mSmartService = configRetrofit(SmartService.class, BASE_URL);
                }
            }
        }
        return mSmartService;
    }

}
