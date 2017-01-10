package com.winorout.cashbook.qmyan.setting;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.winorout.cashbook.R;

/**
 * Created by tom on 2016/12/14.
 */

public class HelpActivity extends Activity implements View.OnClickListener{

    private TextView title;
    private ImageView back;
    private Button message;
    private EditText editText;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_help);

        initView();
        initEvent();
    }

    public void initView(){
        TextView title = (TextView)findViewById(R.id.title);
        title.setText("帮助与反馈");
        back = (ImageView)findViewById(R.id.back);
         message =(Button)findViewById(R.id.leave_message);
        editText = (EditText)findViewById(R.id.editText);


    }

    public void initEvent(){
        back.setOnClickListener(this);
        message.setOnClickListener(this);
    }


    public void onClick(View v){
        switch (v.getId()){
            case R.id.back:
                finish();
                break;
            case R.id.leave_message:
                if (editText.getText().toString().length() != 0) {
                    Toast.makeText(this, "感谢您宝贵的意见", Toast.LENGTH_SHORT).show();
                    finish();
                }
        }

    }
}
