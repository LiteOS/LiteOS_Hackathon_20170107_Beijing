package com.winorout.cashbook.qmyan.register;

import android.text.TextUtils;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class RegisterPresenter implements IRegisterPresenter, IRegisterModel.OnRegisterListener {

    private IRegisterView mView;
    private RegisterModel mRegisterModel;

    public RegisterPresenter(IRegisterView view) {
        mView = view;
        mRegisterModel = new RegisterModel();
    }

    @Override
    public void register(String userName, String userPassword, String email) {
        if (mView != null) {
            if (TextUtils.isEmpty(userName)) {
                mView.onError("用户名不能为空");
                return;
            } else if (TextUtils.isEmpty(userPassword)) {
                mView.onError("密码不能为空");
                return;
            } else if (TextUtils.isEmpty(email)) {
                mView.onError("密保邮箱不能为空");
                return;
            } else {
                mRegisterModel.register(userName, userPassword, email, this);
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
        mView.onError(msg);
    }
}
