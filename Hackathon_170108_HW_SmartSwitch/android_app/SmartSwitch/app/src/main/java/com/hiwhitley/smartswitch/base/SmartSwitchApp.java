package com.hiwhitley.smartswitch.base;

import android.app.Application;

import com.hiwhitley.smartswitch.entity.User;

/**
 * Created by hiwhitley on 2017/1/8.
 */

public class SmartSwitchApp extends Application {
    public User mUser;

    public  User getUser() {
        return mUser;
    }

    public  void setUser(User user) {
        mUser = user;
    }

    public SmartSwitchApp getSmartSwitchApp() {
        return this;
    }
}
