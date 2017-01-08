package com.winorout.cashbook.qmyan.register;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public interface IRegisterModel {
    interface OnRegisterListener {

        void onSuccess();

        void onFailed(String msg);
    }

    void register(String userName, String userPassword, String email, OnRegisterListener listener);
}
