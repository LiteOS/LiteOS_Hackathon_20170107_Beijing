package com.hiwhitley.smartswitch;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


import com.hiwhitley.smartswitch.base.BaseActivity;
import com.hiwhitley.smartswitch.base.SmartSwitchApp;
import com.hiwhitley.smartswitch.entity.ResponseBody;
import com.hiwhitley.smartswitch.entity.User;
import com.hiwhitley.smartswitch.network.RetrofitFactory;

import java.util.Date;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;


/**
 * Created by hiwhitley on 2016/3/29.
 */
public class LoginActivity extends BaseActivity {

    private static final String TAG = "LoginActivity";
    private Button loginBtn;
    private TextView registerBtn;
    private TextView forgetBtn;
    private AutoCompleteTextView mNameTextView;
    private EditText mPswEditText;

    @Override
    protected int getContentViewId() {
        return R.layout.activity_login;
    }

    protected void findViews() {
        loginBtn = findView(R.id.sign_in_button);
        registerBtn = findView(R.id.tv_register);
        forgetBtn = findView(R.id.tv_forget_psw);
        mNameTextView = findView(R.id.email);
        mPswEditText = findView(R.id.password);
    }

    @Override
    protected void init() {
        findViews();
    }

    @Override
    protected void setListeners() {

        loginBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toLogin();
            }
        });

        registerBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), RegisterActivity.class);
                startActivity(intent);
            }
        });

        forgetBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                Intent intent = new Intent(getBaseContext(), MainActivity.class);
//                startActivity(intent);
                bindDevice();
            }
        });

    }

    private void toLogin() {
        String userName = mNameTextView.getText().toString();
        String password = mPswEditText.getText().toString();
        if ("".equals(userName)) {
            return;
        }
        if ("".equals(password)) {
            return;
        }

        Call<User> login = RetrofitFactory.getSmartService().login(userName, password);
        login.enqueue(new Callback<User>() {
            @Override
            public void onResponse(Call<User> call, Response<User> response) {
                Log.d(TAG, "onResponse" + response.body());
                User user = response.body();
                if (user.isSuccessful()) {
                    Intent intent = new Intent(getBaseContext(), MainActivity.class);
                    SmartSwitchApp smartSwitchApp = (SmartSwitchApp) getApplication();
                    smartSwitchApp.setUser(user);
                    startActivity(intent);
                    showToast("登录成功!" );
                } else {
                    showToast("登录失败");
                }
            }

            @Override
            public void onFailure(Call<User> call, Throwable t) {
                Log.d(TAG, "onFailure" + t.getMessage());
                showToast("登录失败:" + t.getMessage());
            }
        });

    }

    private void bindDevice() {
        Call<ResponseBody> call = RetrofitFactory.getSmartService().bindUserAndDev("18868812345", "000");
        call.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                Log.d(TAG, "onResponse" + response.body());
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                Log.d(TAG, "onFailure" + t.getMessage());
            }
        });
    }

}
