package com.winorout.cashbook.xdwan;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.ImageView;

import com.winorout.cashbook.R;

/**
 * Created by Mr-x on 2016/12/12.
 */

public class Target extends Activity implements View.OnClickListener{
    private LinearLayout calculate;
    private TextView settarget;
    private TextView targets;
    private TextView total;
    private TextView day;
    private TextView week;
    private TextView month;
    private ImageView back;
    private Button number0;
    private Button number1;
    private Button number2;
    private Button number3;
    private Button number4;
    private Button number5;
    private Button number6;
    private Button number7;
    private Button number8;
    private Button number9;
    private Button numbersure;
    private Button numberc;
    private static String numbers = "";
    SharedPreferences sharefreference;
    private SharedPreferences.Editor editer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.target);
        initView();
        initEvent();
    }

    private void initView()
    {
        calculate = (LinearLayout)findViewById(R.id.calculate);
        calculate.setVisibility(View.INVISIBLE);
        settarget = (TextView)findViewById(R.id.settarget);
        total = (TextView)findViewById(R.id.total);
        number0 = (Button)findViewById(R.id.number0);
        number1 = (Button)findViewById(R.id.number1);
        number2 = (Button)findViewById(R.id.number2);
        number3 = (Button)findViewById(R.id.number3);
        number4 = (Button)findViewById(R.id.number4);
        number5 = (Button)findViewById(R.id.number5);
        number6 = (Button)findViewById(R.id.number6);
        number7 = (Button)findViewById(R.id.number7);
        number8 = (Button)findViewById(R.id.number8);
        number9 = (Button)findViewById(R.id.number9);
        numberc = (Button)findViewById(R.id.numberc);
        numbersure = (Button)findViewById(R.id.numbersure);
        targets = (TextView)findViewById(R.id.targets);
        day = (TextView)findViewById(R.id.day);
        week = (TextView)findViewById(R.id.week);
        month = (TextView)findViewById(R.id.month);
        back = (ImageView)findViewById(R.id.back);
        sharefreference = getSharedPreferences("loginUser", Context.MODE_PRIVATE);
        editer = sharefreference.edit();
    }

    private void initEvent() {
        settarget.setOnClickListener(this);
        number0.setOnClickListener(this);
        number1.setOnClickListener(this);
        number2.setOnClickListener(this);
        number3.setOnClickListener(this);
        number4.setOnClickListener(this);
        number5.setOnClickListener(this);
        number6.setOnClickListener(this);
        number7.setOnClickListener(this);
        number8.setOnClickListener(this);
        number9.setOnClickListener(this);
        numbersure.setOnClickListener(this);
        numberc.setOnClickListener(this);
        back.setOnClickListener(this);
        setmoney(sharefreference.getString("money", "123"));
    }

    @Override
    public void onClick(View view) {
        switch (view.getId())
        {
            case R.id.settarget:
                calculate.setVisibility(View.VISIBLE);
                break;
            case R.id.number0:
                input(0);
                break;
            case R.id.number1:
                input(1);
                break;
            case R.id.number2:
                input(2);
                break;
            case R.id.number3:
                input(3);
                break;
            case R.id.number4:
                input(4);
                break;
            case R.id.number5:
                input(5);
                break;
            case R.id.number6:
                input(6);
                break;
            case R.id.number7:
                input(7);
                break;
            case R.id.number8:
                input(8);
                break;
            case R.id.number9:
                input(9);
                break;
            case R.id.numberc:
                numbers = "";
                total.setText(numbers);
                break;
            case R.id.back:
                finish();
                break;
            case R.id.numbersure:
                calculate.setVisibility(View.INVISIBLE);
                if (numbers.length() != 0) {
                    targets.setText(numbers);
                    setmoney(numbers);
                    editer.putString("money", numbers);
                    editer.commit();
                    numbers = "";
                }
                break;
        }
    }

    private void setmoney(String number)
    {
//        LayoutInflater layoutInflater = LayoutInflater.from(Target.this);
//        View view = layoutInflater.inflate(R.layout.activity_main, null);
//        TextView gross_income = (TextView)view.findViewById(R.id.gross_income);
//        TextView total_expenditure = (TextView)view.findViewById(R.id.total_expenditure);
//        int money = Integer.parseInt(gross_income.getText().toString());
//        int money1 = Integer.parseInt(total_expenditure.toString().toString());
//        Toast.makeText(this, "" + money, Toast.LENGTH_SHORT).show();
        int many = Integer.parseInt(number);
        day.setText(many / 30 + "");
        week.setText(many / 7 + "");
//        if (money <= money1) {
//            if ((money1 - money) <= Integer.parseInt(number)) {
//                int x = Integer.parseInt(number) - money1 + money;
//                month.setText(" " + x);
//            }
//            else
//            {
//                month.setText("已超额");
//            }
//        }
//        else
//        {
//            month.setText(number);
//        }
        targets.setText(number);
    }

    private void input(int number) {
        numbers += "" + number;
        total.setText(numbers);
    }

    @Override
    protected void onResume() {
        super.onResume();
        calculate.setVisibility(View.INVISIBLE);
    }
}
