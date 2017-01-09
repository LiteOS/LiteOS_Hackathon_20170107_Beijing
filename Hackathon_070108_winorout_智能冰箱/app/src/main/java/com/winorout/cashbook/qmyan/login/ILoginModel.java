package com.winorout.cashbook.qmyan.login;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public interface ILoginModel {

    interface OnLoginListener {

        void onSuccess();

        void onFailed(String msg);
    }

    void login(String userName, String userPassword, OnLoginListener listener);
}
