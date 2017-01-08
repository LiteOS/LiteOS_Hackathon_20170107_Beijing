package com.winorout.cashbook.qmyan.setting;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.winorout.cashbook.R;
import com.winorout.cashbook.qmyan.login.LoginActivity;
import com.winorout.cashbook.qmyan.personcenter.PersonCenter;

public class SettingActivity extends Activity implements View.OnClickListener {


    public static final int FLAG_FROM_SETTINGACTIVITY = 1;

    private TextView title;
    private ImageView back;
    private LinearLayout personCenter;
    private LinearLayout help_layout;
    private LinearLayout update;
    private LinearLayout about;
    private LinearLayout synchronize;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_setting);

        initView();
        initEvent();
    }

    private void initEvent() {
        back.setOnClickListener(this);
        personCenter.setOnClickListener(this);
        help_layout.setOnClickListener(this);
        update.setOnClickListener(this);
        about.setOnClickListener(this);
        synchronize.setOnClickListener(this);
    }

    private void initView() {
        title = (TextView) findViewById(R.id.title);
        title.setText("设置");
        back = (ImageView) findViewById(R.id.back);
        personCenter = (LinearLayout) findViewById(R.id.person_center);
        help_layout = (LinearLayout) findViewById(R.id.help_layout);
        update = (LinearLayout) findViewById(R.id.update_btn);
        about = (LinearLayout) findViewById(R.id.about_btn);
        synchronize = (LinearLayout) findViewById(R.id.synchronize);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.back:
                finish();
                break;
            case R.id.person_center:
                SharedPreferences mSharedPreferences = getSharedPreferences("loginUser", Context.MODE_PRIVATE);
                Boolean isLogin = mSharedPreferences.getBoolean("isLogin", false);
                if (isLogin) {
                    Intent intent = new Intent(this, PersonCenter.class);
                    intent.setFlags(FLAG_FROM_SETTINGACTIVITY);
                    startActivity(intent);
                    finish();
                } else {
                    Toast.makeText(this, "请先登录", Toast.LENGTH_SHORT).show();
                    startActivity(new Intent(this, LoginActivity.class));
                    finish();
                }
                break;
            case R.id.help_layout:
                startActivity(new Intent(this, HelpActivity.class));
                finish();
                break;
            case R.id.update_btn:
                Toast.makeText(this, "当前已经是最新版", Toast.LENGTH_SHORT).show();
                break;
            case R.id.about_btn:
                startActivity(new Intent(this, AboutActivity.class));
                finish();
                break;
            case R.id.synchronize:
                Toast.makeText(this, "同步中...", Toast.LENGTH_SHORT).show();
                break;
        }
    }
}
