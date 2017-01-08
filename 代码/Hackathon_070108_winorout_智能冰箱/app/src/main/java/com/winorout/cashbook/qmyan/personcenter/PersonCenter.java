package com.winorout.cashbook.qmyan.personcenter;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.winorout.cashbook.R;
import com.winorout.cashbook.main.MainActivity;
import com.winorout.cashbook.qmyan.changepwd.ChangePwdActivity;
import com.winorout.cashbook.qmyan.setting.SettingActivity;

import static com.winorout.cashbook.main.MainActivity.FLAG_FROM_MAINACTIVITY;

public class PersonCenter extends Activity implements View.OnClickListener {

    private TextView title;
    private ImageView back;
    private TextView exit;
    private TextView userName;
    private TextView sex;
    private TextView changePwd;
    private SharedPreferences mPreferences;
    private Intent mIntent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_person_center);

        initView();
        initEvent();
    }

    private void initEvent() {
        back.setOnClickListener(this);
        exit.setOnClickListener(this);
        changePwd.setOnClickListener(this);
    }

    private void initView() {
        mPreferences = getSharedPreferences("loginUser", Context.MODE_PRIVATE);
        mIntent = getIntent();
        title = (TextView) findViewById(R.id.title);
        title.setText("个人中心");
        back = (ImageView) findViewById(R.id.back);
        exit = (TextView) findViewById(R.id.exit);
        exit.setVisibility(View.VISIBLE);
        userName = (TextView) findViewById(R.id.user_name);
        userName.setText(mPreferences.getString("userName", "???"));
        sex = (TextView) findViewById(R.id.sex);
        sex.setText(mPreferences.getString("sex", "男"));
        changePwd = (TextView) findViewById(R.id.change_password);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.back:
                back();
                break;
            case R.id.exit:
                SharedPreferences.Editor editor = getSharedPreferences("loginUser", Context.MODE_PRIVATE).edit();
                editor.putBoolean("isLogin", false);
                editor.commit();
                finish();
                break;
            case R.id.change_password:
                startActivity(new Intent(this, ChangePwdActivity.class));
                finish();
                break;
        }
    }

    private void back() {
        if (mIntent.getFlags() == FLAG_FROM_MAINACTIVITY) {
            finish();
        } else {
            startActivity(new Intent(this, SettingActivity.class));
            finish();
        }
    }

    @Override
    public void onBackPressed() {
        back();
    }

}
