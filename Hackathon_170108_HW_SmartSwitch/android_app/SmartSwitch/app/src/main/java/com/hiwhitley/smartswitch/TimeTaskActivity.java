package com.hiwhitley.smartswitch;

import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.TextView;

import com.hiwhitley.smartswitch.base.BaseActivity;

/**
 * Created by hiwhitley on 2017/1/8.
 */

public class TimeTaskActivity extends BaseActivity {
    private Toolbar mToolbar;
    private TextView mTlTextView;
    @Override
    protected int getContentViewId() {
        return R.layout.activity_time_task;
    }

    @Override
    protected void init() {
        mToolbar = findView(R.id.custom_toolbar);
        mTlTextView = findView(R.id.tl_title);
        initTootBar(mToolbar);
        mTlTextView.setText("定时任务" +
                "");
    }

    @Override
    protected void setListeners() {

    }

    public void initTootBar(Toolbar mCustomToolbar) {
        mCustomToolbar.setTitle("");
        mCustomToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}
