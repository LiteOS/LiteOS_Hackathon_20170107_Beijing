package com.hiwhitley.smartswitch.base;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.SnapHelper;
import android.view.View;
import android.view.Window;
import android.widget.Toast;

/**
 * Created by hiwhitley on 2016/3/29.
 */
public abstract class BaseActivity extends AppCompatActivity {


    protected abstract int getContentViewId();

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(getContentViewId());
        init();
        setListeners();
    }

    protected abstract void init();

    protected abstract void setListeners();

    public <T extends View> T findView(int resId) {
        return (T) findViewById(resId);
    }

    public void showToast(String msg) {
        Toast.makeText(getBaseContext(), msg, Toast.LENGTH_LONG).show();
    }
}
