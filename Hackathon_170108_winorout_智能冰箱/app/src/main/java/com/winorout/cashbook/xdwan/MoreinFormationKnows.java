package com.winorout.cashbook.xdwan;

import android.app.Activity;
import android.os.Bundle;

import com.winorout.cashbook.R;

import android.view.View;
import android.view.Window;
import android.widget.TextView;

/**
 * Created by Mr-x on 2017/01/07.
 */

public class MoreinFormationKnows extends Activity implements View.OnClickListener{
    private TextView backs;
    private TextView connect;
    private TextView electricity;
    private TextView refrigeratornumber;
    private boolean connection;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.moreinformationknows);
        initValue();
        initEvent();
        initdata();
    }


    private void initValue() {
        backs = (TextView)findViewById(R.id.backs);
        connect = (TextView)findViewById(R.id.connect);
        electricity = (TextView)findViewById(R.id.electricity);
        refrigeratornumber = (TextView)findViewById(R.id.refrigeratornumber);
    }

    private void initEvent() {
        backs.setOnClickListener(this);
    }

    private void initdata() {
        if (!connection) {
            connect.setText("否");
            electricity.setText("暂无");
            refrigeratornumber.setText("0台");
        }
        else
        {

            connect.setText("是");
            electricity.setText("200W");
            refrigeratornumber.setText("1台");
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId())
        {
            case R.id.backs:
                finish();
                break;
        }
    }
}
