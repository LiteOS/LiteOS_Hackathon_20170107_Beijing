package com.winorout.cashbook.accounting;

import android.app.Fragment;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.winorout.cashbook.R;

/**
 * Created by tom on 2016/12/9.
 */

public class IncomeFragment extends Fragment implements View.OnClickListener {

    private View view;
    private ImageView type_pic;
    private TextView type_word;
    private Button salary_btn;
    private Button red_btn;
    private Button bonus_btn;
    private Button interest_btn;
    private Button pension_btn;
    private Button life_btn;
    private Button extra_income_btn;
    private Button investment_btn;
    private Button income_other_btn;
    private int typePic;
    private SharedPreferences.Editor mEditor;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.income_fragment, container, false);

        initView();
        initEvent();
        return view;
    }

    // 初始化组件
    public void initView() {
        type_word = (TextView) getActivity().findViewById(R.id.type_word);
        type_pic = (ImageView) getActivity().findViewById(R.id.type_pic);
        salary_btn = (Button) view.findViewById(R.id.salary_btn);
        red_btn = (Button) view.findViewById(R.id.red_btn);
        bonus_btn = (Button) view.findViewById(R.id.bonus_btn);
        interest_btn = (Button) view.findViewById(R.id.interest_btn);
        pension_btn = (Button) view.findViewById(R.id.pension_btn);
        life_btn = (Button) view.findViewById(R.id.life_btn);
        extra_income_btn = (Button) view.findViewById(R.id.extra_income_btn);
        investment_btn = (Button) view.findViewById(R.id.investment_btn);
        income_other_btn = (Button) view.findViewById(R.id.income_other_btn);

        mEditor = getActivity().getApplicationContext().getSharedPreferences("loginUser", Context.MODE_PRIVATE).edit();
    }

    // 初始化事件
    public void initEvent() {
        salary_btn.setOnClickListener(this);
        red_btn.setOnClickListener(this);
        bonus_btn.setOnClickListener(this);
        interest_btn.setOnClickListener(this);
        pension_btn.setOnClickListener(this);
        life_btn.setOnClickListener(this);
        extra_income_btn.setOnClickListener(this);
        investment_btn.setOnClickListener(this);
        income_other_btn.setOnClickListener(this);


    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.salary_btn:

                type_pic.setImageResource(typePic = R.drawable.salary_png);

                type_word.setText("工资");
                break;
            case R.id.red_btn:
                type_pic.setImageResource(typePic = R.drawable.red_png);
                type_word.setText("红包");
                break;
            case R.id.bonus_btn:
                type_pic.setImageResource(typePic = R.drawable.bonus_png);
                type_word.setText("奖金");
                break;
            case R.id.interest_btn:
                type_pic.setImageResource(typePic = R.drawable.interest_png);
                type_word.setText("利息");
                break;
            case R.id.pension_btn:
                type_pic.setImageResource(typePic = R.drawable.pension_png);
                type_word.setText("养老金");
                break;
            case R.id.life_btn:
                type_pic.setImageResource(typePic = R.drawable.life_png);
                type_word.setText("生活");
                break;
            case R.id.extra_income_btn:
                type_pic.setImageResource(typePic = R.drawable.extra_income_png);
                type_word.setText("外快");
                break;
            case R.id.investment_btn:
                type_pic.setImageResource(typePic = R.drawable.investment_png);
                type_word.setText("投资");
                break;
            case R.id.income_other_btn:
                type_pic.setImageResource(typePic = R.drawable.other_png);
                type_word.setText("其他");
                break;

            default:
                break;


        }


        mEditor.putInt("typePic", typePic);
        mEditor.commit();


    }

}
