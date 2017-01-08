package com.winorout.cashbook.qmyan.changepwd;

import android.text.TextUtils;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class ChangePwdPresenter implements IChangePwdPresenter, IChangepwdModel.OnChangePwdListener {

    private IChangePwdView mView;
    private ChangepwdModel mChangepwdModel;

    public ChangePwdPresenter(IChangePwdView view) {
        mView = view;
        mChangepwdModel = new ChangepwdModel();
    }

    @Override
    public void changePwd(String userPassword, String email) {
        if (mView != null) {
            if (TextUtils.isEmpty(userPassword)) {
                mView.onError("密码不能为空");
                return;
            } else if (TextUtils.isEmpty(email)) {
                mView.onError("密保邮箱不能为空");
                return;
            } else {
                mChangepwdModel.changePwd(userPassword, email, this);
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
            mView.navigateToLogin();
        }
    }

    @Override
    public void onFailed(String msg) {
        mView.onError(msg);
    }
}
