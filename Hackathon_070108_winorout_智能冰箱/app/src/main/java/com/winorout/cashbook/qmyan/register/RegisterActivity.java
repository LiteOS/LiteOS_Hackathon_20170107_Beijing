package com.winorout.cashbook.qmyan.register;

import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.winorout.cashbook.R;

public class RegisterActivity extends Activity implements IRegisterView, View.OnClickListener {

    private TextView title;
    private TextView userName;
    private TextView userPassword;
    private TextView emali;
    private ImageView back;
    private Button registerBtn;
    private IRegisterPresenter mRegisterPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_register);

        initView();
        initEvent();
    }

    private void initView() {
        title = (TextView) findViewById(R.id.title);
        title.setText("用户注册");
        userName = (TextView) findViewById(R.id.user_name);
        userPassword = (TextView) findViewById(R.id.user_password);
        emali = (TextView) findViewById(R.id.email);
        back = (ImageView) findViewById(R.id.back);
        registerBtn = (Button) findViewById(R.id.register_btn);

        mRegisterPresenter = new RegisterPresenter(this);
    }

    private void initEvent() {
        back.setOnClickListener(this);
        registerBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.register_btn:
                mRegisterPresenter.register(userName.getText().toString()
                        , userPassword.getText().toString()
                        , emali.getText().toString());
                break;
            case R.id.back:
                finish();
                break;
        }
    }

    @Override
    public void onError(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void navigateToHome() {
        Toast.makeText(this, "注册成功", Toast.LENGTH_SHORT).show();
        finish();
    }
}
