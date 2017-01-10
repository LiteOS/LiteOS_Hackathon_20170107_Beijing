package com.winorout.cashbook.qmyan.register;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public interface IRegisterPresenter {

    void register(String userName, String userPassword, String email);

    void destory();
}
