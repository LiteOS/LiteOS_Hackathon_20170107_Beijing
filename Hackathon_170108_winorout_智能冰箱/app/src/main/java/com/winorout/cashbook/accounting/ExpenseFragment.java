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

public class ExpenseFragment extends Fragment implements View.OnClickListener {

    // 声明变量
    private View view;
    private ImageView type_pic;
    private Button cloth_btn;
    private Button communication_btn;
    private Button daily_btn;
    private Button live_btn;
    private Button food_btn;
    private Button friendship_btn;
    private Button house_btn;
    private Button medecine_btn;
    private Button traffic_btn;
    private Button other_btn;
    public TextView type_word;
    private int typePic;
    private SharedPreferences.Editor mEditor;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.expense_fragment, container, false);

        initView();
        initEvent();

        return view;
    }

    // 初始化组件
    public void initView() {
        type_word = (TextView)getActivity().findViewById(R.id.type_word);
        type_pic = (ImageView) getActivity().findViewById(R.id.type_pic);
        cloth_btn = (Button) view.findViewById(R.id.cloth_btn);
        communication_btn = (Button) view.findViewById(R.id.communication_btn);
        daily_btn = (Button) view.findViewById(R.id.daily_btn);
        live_btn = (Button) view.findViewById(R.id.live_btn);
        food_btn = (Button) view.findViewById(R.id.food_btn);
        friendship_btn = (Button) view.findViewById(R.id.friendship_btn);
        house_btn = (Button) view.findViewById(R.id.house_btn);
        medecine_btn = (Button) view.findViewById(R.id.medicine_btn);
        traffic_btn = (Button) view.findViewById(R.id.traffic_btn);
        other_btn = (Button) view.findViewById(R.id.other_btn);
        mEditor = getActivity().getApplicationContext().getSharedPreferences("loginUser", Context.MODE_PRIVATE).edit();

    }

    // 初始化事件
    public void initEvent() {
        cloth_btn.setOnClickListener(this);
        communication_btn.setOnClickListener(this);
        daily_btn.setOnClickListener(this);
        live_btn.setOnClickListener(this);
        food_btn.setOnClickListener(this);
        friendship_btn.setOnClickListener(this);
        house_btn.setOnClickListener(this);
        medecine_btn.setOnClickListener(this);
        traffic_btn.setOnClickListener(this);
        other_btn.setOnClickListener(this);

    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.cloth_btn:

                type_pic.setImageResource(typePic = R.drawable.cloth_png);
                type_word.setText("衣服");
                break;
            case R.id.communication_btn:
                type_pic.setImageResource(typePic = R.drawable.communication_png);
                type_word.setText("通讯");
                break;
            case R.id.daily_btn:
                type_pic.setImageResource(typePic = R.drawable.daily);
                type_word.setText("日用品");
                break;
            case R.id.live_btn:
                type_pic.setImageResource(typePic = R.drawable.live_png);
                type_word.setText("生活");
                break;
            case R.id.food_btn:
                type_pic.setImageResource(typePic = R.drawable.food_png);
                type_word.setText("食品");
                break;
            case R.id.friendship_btn:
                type_pic.setImageResource(typePic = R.drawable.friendship_png);
                type_word.setText("人情");
                break;
            case R.id.house_btn:
                type_pic.setImageResource(typePic = R.drawable.house_png);
                type_word.setText("房租");
                break;
            case R.id.medicine_btn:
                type_pic.setImageResource(typePic = R.drawable.medicine_png);
                type_word.setText("医药");
                break;
            case R.id.traffic_btn:
                type_pic.setImageResource(typePic = R.drawable.traffic_png);
                type_word.setText("交通");
                break;
            case R.id.other_btn:
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
