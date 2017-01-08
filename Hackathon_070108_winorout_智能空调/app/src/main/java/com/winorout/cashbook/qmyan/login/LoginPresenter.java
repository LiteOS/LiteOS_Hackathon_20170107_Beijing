package com.winorout.cashbook.qmyan.login;

import android.text.TextUtils;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class LoginPresenter implements ILoginPresenter, ILoginModel.OnLoginListener {

    private ILoginView mView;
    private LoginModel mLoginModel;

    public LoginPresenter(ILoginView view) {
        mView = view;
        mLoginModel = new LoginModel(((LoginActivity)mView).getApplicationContext());
    }

    @Override
    public void login(String userName, String userPassword) {
        if (mView != null) {
            if (TextUtils.isEmpty(userName)) {
                mView.onError("用户名不能为空");
                return;
            } else if (TextUtils.isEmpty(userPassword)) {
                mView.onError("密码不能为空");
                return;
            } else {
                mLoginModel.login(userName, userPassword, this);
            }
        }
    }

    @Override
    public void destory() {
        if (mView != null) {
            mView = null;
        }
    }

    @Override
    public void onSuccess() {
        if (mView != null) {
            mView.navigateToHome();
        }
    }

    @Override
    public void onFailed(String msg) {
        if (mView != null) {
            mView.onError(msg);
        }
    }
}
