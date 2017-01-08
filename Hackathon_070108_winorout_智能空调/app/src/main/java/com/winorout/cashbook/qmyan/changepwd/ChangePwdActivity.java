package com.winorout.cashbook.qmyan.changepwd;

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
import com.winorout.cashbook.qmyan.login.LoginActivity;

public class ChangePwdActivity extends Activity implements IChangePwdView, View.OnClickListener {

    private TextView title;
    private TextView userPassword;
    private TextView email;
    private ImageView back;
    private Button changePwd;

    private ChangePwdPresenter mChangePwdPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_change_pwd);

        initView();
        initEvent();
    }

    private void initEvent() {
        back.setOnClickListener(this);
        changePwd.setOnClickListener(this);
    }

    private void initView() {
        title = (TextView) findViewById(R.id.title);
        title.setText("密码修改");
        userPassword = (TextView) findViewById(R.id.user_password);
        email = (TextView) findViewById(R.id.email);
        back = (ImageView) findViewById(R.id.back);
        changePwd = (Button) findViewById(R.id.change_password);

        mChangePwdPresenter = new ChangePwdPresenter(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mChangePwdPresenter.destory();
    }

    @Override
    public void navigateToLogin() {
        Toast.makeText(this, "密码修改成功", Toast.LENGTH_SHORT).show();
        startActivity(new Intent(this, LoginActivity.class));
        finish();
    }

    @Override
    public void onError(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.back:
                finish();
                break;
            case R.id.change_password:
                mChangePwdPresenter.changePwd(userPassword.getText().toString(), email.getText().toString());
                break;
        }
    }
}
