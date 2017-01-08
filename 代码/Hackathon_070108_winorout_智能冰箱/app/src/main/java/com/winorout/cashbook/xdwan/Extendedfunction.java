package com.winorout.cashbook.xdwan;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;

import com.winorout.cashbook.R;

import android.widget.TextView;
import android.widget.Toast;

/**
 * Created by Mr-x on 2017/01/07.
 */

public class Extendedfunction extends Activity implements View.OnClickListener {
    private TextView moreinformationknows;
    private TextView backtomain;
    private TextView check;
    private TextView model;
    private TextView clear;
    private TextView smell;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.extendedfunction);
        iniVaules();
        iniEvent();
    }

    private void iniVaules() {
        moreinformationknows = (TextView) findViewById(R.id.moreinformationknows);
        backtomain = (TextView) findViewById(R.id.backtomain);
        check = (TextView) findViewById(R.id.check);
        model = (TextView) findViewById(R.id.model);
        clear = (TextView)findViewById(R.id.clear);
        smell = (TextView)findViewById(R.id.smell);
    }

    private void iniEvent() {
        moreinformationknows.setOnClickListener(this);
        backtomain.setOnClickListener(this);
        check.setOnClickListener(this);
        model.setOnClickListener(this);
        clear.setOnClickListener(this);
        smell.setOnClickListener(this);
    }


    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.moreinformationknows:
                Intent intent = new Intent(Extendedfunction.this, MoreinFormationKnows.class);
                startActivity(intent);
                break;
            case R.id.backtomain:
                finish();
                break;
            case R.id.check:
                Toast.makeText(this, "请先连接冰箱", Toast.LENGTH_SHORT).show();
                break;
            case R.id.model:
                Toast.makeText(this, "请先连接冰箱", Toast.LENGTH_SHORT).show();
                break;
            case R.id.clear:
                Toast.makeText(this, "请先连接冰箱", Toast.LENGTH_SHORT).show();
                break;
            case R.id.smell:
                Toast.makeText(this, "请先连接冰箱", Toast.LENGTH_SHORT).show();
                break;
        }
    }
}
