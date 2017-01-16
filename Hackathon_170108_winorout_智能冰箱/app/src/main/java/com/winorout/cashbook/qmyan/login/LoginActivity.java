package com.winorout.cashbook.qmyan.login;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.winorout.cashbook.R;
import com.winorout.cashbook.qmyan.changepwd.ChangePwdActivity;
import com.winorout.cashbook.qmyan.register.RegisterActivity;

public class LoginActivity extends Activity implements ILoginView, View.OnClickListener {

    private TextView title;
    private TextView userName;
    private TextView userPassword;
    private TextView changePassword;
    private ImageView back;
    private Button loginBtn;
    private Button registerBtn;
    private LoginPresenter mLoginPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_login);

        initView();
        initEvent();
    }

    private void initEvent() {
        back.setOnClickListener(this);
        changePassword.setOnClickListener(this);
        loginBtn.setOnClickListener(this);
        registerBtn.setOnClickListener(this);
    }

    private void initView() {
        title = (TextView) findViewById(R.id.title);
        title.setText("用户登录");
        userName = (TextView) findViewById(R.id.user_name);
        userPassword = (TextView) findViewById(R.id.user_password);
        changePassword = (TextView) findViewById(R.id.change_password);
        back = (ImageView) findViewById(R.id.back);
        loginBtn = (Button) findViewById(R.id.login_btn);
        registerBtn = (Button) findViewById(R.id.register_btn);

        mLoginPresenter = new LoginPresenter(this);
    }

    @Override
    public void onError(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void navigateToHome() {
        Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show();
        finish();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.login_btn:
                mLoginPresenter.login(userName.getText().toString(), userPassword.getText().toString());
                break;
            case R.id.register_btn:
                startActivity(new Intent(this, RegisterActivity.class));
                break;
            case R.id.change_password:
                startActivity(new Intent(this, ChangePwdActivity.class));
                break;
            case R.id.back:
                finish();
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mLoginPresenter.destory();
    }
}
