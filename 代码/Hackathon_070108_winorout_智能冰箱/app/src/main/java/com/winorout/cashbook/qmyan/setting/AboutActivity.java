package com.winorout.cashbook.qmyan.setting;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;

import android.widget.ImageView;
import android.widget.TextView;


import com.winorout.cashbook.R;

/**
 * Created by tom on 2016/12/14.
 */

public class AboutActivity extends Activity implements View.OnClickListener{

    private ImageView back;


    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_aboutus);

        initView();
        initEvent();
    }

    public void initView(){
        TextView title = (TextView)findViewById(R.id.title);
        title.setText("关于我们");
        back = (ImageView)findViewById(R.id.back);


    }

    public void initEvent(){
        back.setOnClickListener(this);

    }


    public void onClick(View v){
        switch (v.getId()){
            case R.id.back:
                finish();
                break;


        }

    }

}
