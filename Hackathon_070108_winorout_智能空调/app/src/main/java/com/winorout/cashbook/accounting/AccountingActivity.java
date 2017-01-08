package com.winorout.cashbook.accounting;

import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.ContentValues;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.winorout.cashbook.R;

import java.util.Date;

import static com.winorout.cashbook.common.MyApplication.dbHelper;

/**
 * Created by ZhangMin on 2016/12/04.
 */

public class AccountingActivity extends Activity implements View.OnClickListener {
    private ExpenseFragment expenseFragment = new ExpenseFragment();
    private IncomeFragment incomeFragment = new IncomeFragment();

    private EditText note_edit;
    private Button save_btn;
    private LinearLayout income_btn;
    private LinearLayout expense_btn;
    private LinearLayout back_btn;
    private Button clean_btn;
    private Button one_btn;
    private Button two_btn;
    private Button three_btn;
    private Button four_btn;
    private Button five_btn;
    private Button six_btn;
    private Button seven_btn;
    private Button eight_btn;
    private Button nine_btn;
    private Button zero_btn;
    private Button dot_btn;
    private Button add_btn;
    private Button minus_btn;
    private TextView type_amount;   // 计算结果
    private double type_acount_init = 0.0;   // 计算过程需要用到的结果，最近一个符号后的结果，而不是当前的结果
    private TextView type_calculate;
    private int add_time = 0;           //加法的次数
    private int minus_time = 0;
    private int or = 0;     //判断加法或者减法
    private int inorout = 1;
    private String categoryType;
    private String categoryName;
    private TextView type_word;
    private ImageView type_pic;
    private SharedPreferences sharedPreferences;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_accounting);
        FragmentManager fragmentManager = getFragmentManager();
        FragmentTransaction transaction0 = fragmentManager.beginTransaction();
        transaction0.replace(R.id.classify, expenseFragment);
        transaction0.commit();

        initView();
        initEvent();

    }

    public void initView() {
        note_edit = (EditText)findViewById(R.id.note_edit);
        type_pic = (ImageView)findViewById(R.id.type_pic);
        type_word = (TextView)findViewById(R.id.type_word);
        zero_btn = (Button) findViewById(R.id.zero_btn);
        save_btn = (Button) findViewById(R.id.save_btn);
        add_btn = (Button) findViewById(R.id.add_btn);
        minus_btn = (Button) findViewById(R.id.minus_btn);
        one_btn = (Button) findViewById(R.id.one_btn);
        two_btn = (Button) findViewById(R.id.two_btn);
        three_btn = (Button) findViewById(R.id.three_btn);
        four_btn = (Button) findViewById(R.id.four_btn);
        five_btn = (Button) findViewById(R.id.five_btn);
        six_btn = (Button) findViewById(R.id.six_btn);
        seven_btn = (Button) findViewById(R.id.seven_btn);
        eight_btn = (Button) findViewById(R.id.eight_btn);
        nine_btn = (Button) findViewById(R.id.nine_btn);
        dot_btn = (Button) findViewById(R.id.dot_btn);
        type_amount = (TextView) findViewById(R.id.type_amount);
        type_calculate = (TextView) findViewById(R.id.type_calculate);
        clean_btn = (Button) findViewById(R.id.clean_btn);
        income_btn = (LinearLayout) findViewById(R.id.income_layout);
        expense_btn = (LinearLayout) findViewById(R.id.expense_layout);
        back_btn = (LinearLayout) findViewById(R.id.back_btn);

        sharedPreferences = getSharedPreferences("loginUser", Context.MODE_PRIVATE);
    }


    public void initEvent() {
        clean_btn.setOnClickListener(this);
        save_btn.setOnClickListener(this);
        income_btn.setOnClickListener(this);
        expense_btn.setOnClickListener(this);
        back_btn.setOnClickListener(this);
        add_btn.setOnClickListener(this);
        minus_btn.setOnClickListener(this);
        one_btn.setOnClickListener(this);
        two_btn.setOnClickListener(this);
        three_btn.setOnClickListener(this);
        four_btn.setOnClickListener(this);
        five_btn.setOnClickListener(this);
        six_btn.setOnClickListener(this);
        seven_btn.setOnClickListener(this);
        eight_btn.setOnClickListener(this);
        nine_btn.setOnClickListener(this);
        zero_btn.setOnClickListener(this);
        add_btn.setOnClickListener(this);
        minus_btn.setOnClickListener(this);
        dot_btn.setOnClickListener(this);

    }


    // 计算器加法方法
    public void add() {
        try {
            String[] i = type_calculate.getText().toString().split("\\+");
            Double y = Double.parseDouble(i[add_time]);
            type_amount.setText(String.valueOf(type_acount_init + y));
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(getApplicationContext(), "用户非法操作", Toast.LENGTH_SHORT).show();
            clear();
        }

    }

    // 计算器减法方法
    public void minus() {
        try {
            String[] i = type_calculate.getText().toString().split("\\-");
            Double y = Double.parseDouble(i[minus_time]);
            type_amount.setText(String.valueOf(type_acount_init - y));
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(getApplicationContext(), "用户非法操作", Toast.LENGTH_SHORT).show();
            clear();
        }
    }


    @Override
    public void onClick(View v) {
        try {
            switch (v.getId()) {
                case R.id.income_layout:
                    FragmentTransaction transaction = getFragmentManager().beginTransaction();
                    transaction.replace(R.id.classify, incomeFragment);
                    transaction.show(incomeFragment);
                    transaction.hide(expenseFragment);
                    transaction.commit();
                    inorout = 2;
                    type_pic.setImageResource(R.drawable.salary_png);
                    type_word.setText("工资");
                    break;

                case R.id.expense_layout:
                    FragmentTransaction transaction1 = getFragmentManager().beginTransaction();
                    transaction1.replace(R.id.classify, expenseFragment);
                    transaction1.show(expenseFragment);
                    transaction1.hide(incomeFragment);
                    transaction1.commit();
                    inorout = 1;
                    type_pic.setImageResource(R.drawable.cloth_png);
                    type_word.setText("衣服");
                    break;
                case R.id.back_btn:
                    finish();
                    break;

                case R.id.clean_btn:
                    clear();
                    add_time = 0;
                    minus_time = 0;
                    type_acount_init = 0.0;

                    break;

                case R.id.one_btn:
//                if(add_time == 0){
//                    type_amount.append("1");
//                }
//                else if(add_time == 1){
//                    type_calculate.append("1");
////                    String i = type_calculate.getText().subSequence(0,3).toString();
////                    Integer.parseInt(i);
//                    String[] i = type_calculate.getText().toString().split("\\+");
//                    int y = Integer.parseInt(i[0])+Integer.parseInt(i[1]);
//                    type_amount.setText(String.valueOf(y));
//
//
//                }else if(add_time == 2){
//                    type_calculate.append("1");
////                    String i = type_calculate.getText().subSequence(0,3).toString();
////                    Integer.parseInt(i);
//                    String[] i = type_calculate.getText().toString().split("\\+");
//                    int y = Integer.parseInt(i[0])+Integer.parseInt(i[1])+Integer.parseInt(i[2]);
//                    type_amount.setText(String.valueOf(y));
//                }

                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);
                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("1");
                    } else if (or == 0) {
                        type_calculate.append("1");
                        add();
                    } else {
                        type_calculate.append("1");
                        minus();
                    }
                    break;


                case R.id.two_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);
                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("2");
                    } else if (or == 0) {


                        type_calculate.append("2");
                        add();
                    } else {
                        type_calculate.append("2");
                        minus();
                    }
                    break;

                case R.id.three_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("3");
                    } else if (or == 0) {
                        type_calculate.append("3");
                        add();
                    } else {
                        type_calculate.append("3");
                        minus();
                    }

                    break;


                case R.id.four_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("4");
                    } else if (or == 0) {
                        type_calculate.append("4");
                        add();
                    } else {
                        type_calculate.append("4");
                        minus();
                    }

                    break;

                case R.id.five_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("5");
                    } else if (or == 0) {
                        type_calculate.append("5");
                        add();
                    } else {
                        type_calculate.append("5");
                        minus();
                    }
                    break;


                case R.id.six_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("6");
                    } else if (or == 0) {
                        type_calculate.append("6");
                        add();
                    } else {
                        type_calculate.append("6");
                        minus();
                    }

                    break;

                case R.id.seven_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("7");
                    } else if (or == 0) {
                        type_calculate.append("7");
                        add();
                    } else {
                        type_calculate.append("7");
                        minus();
                    }
                    break;

                case R.id.eight_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("8");
                    } else if (or == 0) {
                        type_calculate.append("8");
                        add();
                    } else {
                        type_calculate.append("8");
                        minus();
                    }

                    break;


                case R.id.nine_btn:
                    add_btn.setClickable(true);
                    minus_btn.setClickable(true);
                    dot_btn.setClickable(true);
                    zero_btn.setClickable(true);

                    if (add_time == 0 && minus_time == 0) {
                        type_amount.append("9");
                    } else if (or == 0) {
                        type_calculate.append("9");
                        add();

                    } else {
                        type_calculate.append("9");
                        minus();
                    }

                    break;

                case R.id.zero_btn:
                    if (type_amount.getText().toString().length() == 0) {
                        zero_btn.setClickable(false);
                        break;
                    } else if (add_time == 0 && minus_time == 0) {
                        type_amount.append("0");
                    } else if (or == 0) {
                        type_calculate.append("0");
                        add();
                    } else {
                        type_calculate.append("0");
                        minus();
                    }

                    break;

                case R.id.dot_btn:
                    if (type_amount.getText().toString().length() == 0) {
                        dot_btn.setClickable(false);
                        break;
                    } else if (add_time == 0 && minus_time == 0) {
                        type_amount.append(".");
                    } else {
                        type_calculate.append(".");
                    }

                    break;

                case R.id.add_btn:
                    if (type_amount.getText().toString().length() == 0) {
                        add_btn.setClickable(false);
                        break;
                    } else if (add_time == 0 && minus_time == 0) {
                        type_calculate.append(type_amount.getText().toString() + "+");
                    } else {
                        type_calculate.append("+");
                    }
                    add_time++;
                    or = 0;

                    type_acount_init = Double.parseDouble(type_amount.getText().toString());
                    break;

                case R.id.minus_btn:

                    if (type_amount.getText().toString().length() == 0) {
                        minus_btn.setClickable(false);
                        break;
                    } else if (add_time == 0 && minus_time == 0) {
                        type_calculate.append(type_amount.getText().toString() + "-");
                    } else {
                        type_calculate.append("-");
                    }
                    minus_time++;
                    or = 1;
                    type_acount_init = Double.parseDouble(type_amount.getText().toString());
                    break;

                case R.id.save_btn:


                    if(inorout == 1){
                        categoryType = "支出";
                    }else{
                        categoryType = "收入";
                    }


                    String userName = sharedPreferences.getString("userName", "default");

                    // TODO: 2016/12/13 判断
                    if(userName == "default"){
                        Toast.makeText(getApplicationContext(),"请登录",Toast.LENGTH_SHORT).show();
                    }else {

                        SQLiteDatabase db = dbHelper.getWritableDatabase();
                        ContentValues values = new ContentValues();
                        values.put("note",note_edit.getText().toString());
                        values.put("amount",type_amount.getText().toString());
                        values.put("userId", userName);
                        values.put("categoryType", categoryType);
                        values.put("categoryName", categoryName);
                        values.put("typePic",sharedPreferences.getInt("typePic", R.drawable.bonus_png));
                        Log.d("TAG", "typePic " + sharedPreferences.getInt("typePic", R.drawable.bonus_png));
                        Date date = new Date();
                        Log.d("TAG", "date.toString: " + date.toString());
                        values.put("date", date.toString());
                        db.insert("Finace", null, values);
                        db.close();
                    }

                    finish();


                    break;

                default:
                    break;
//


            }
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(getApplicationContext(), "用户非法操作", Toast.LENGTH_SHORT).show();
            clear();
        }




    }

    private void clear() {
        type_amount.setText("");
        type_calculate.setText("");
    }


}
