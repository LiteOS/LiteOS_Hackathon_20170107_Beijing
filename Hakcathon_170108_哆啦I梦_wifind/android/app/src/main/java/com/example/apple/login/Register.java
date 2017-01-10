package com.example.apple.login;

import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TableRow;
import android.widget.Toast;

import cn.smssdk.EventHandler;
import cn.smssdk.SMSSDK;

public class Register extends AppCompatActivity {

    private int time = 60;
    private boolean flag = true;
    private boolean validate = false;

    Button submit;
    Button cancel;
    Button check;

    EditText username;
    EditText pass1;
    EditText pass2;
    EditText phone;
    EditText email;
    EditText checkcode;

    TableRow checkCodeRow;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        INIT();

        SMSSDK.initSDK(this, "1a6efa9a24051", "bd3d7d961900508029224d9dff24c193");

        EventHandler eh=new EventHandler(){
            @Override

            public void afterEvent(int event, int result, Object data) {

                Message msg = new Message();
                msg.arg1 = event;
                msg.arg2 = result;
                msg.obj = data;
                handler.sendMessage(msg);
            }
        };

        SMSSDK.registerEventHandler(eh);

        check.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                String ph = phone.getText().toString();
                if (ph.equals("")){
                    Toast.makeText(Register.this, "手机号不能为空", Toast.LENGTH_SHORT).show();
                }
                else {
                    time = 60;
                    SMSSDK.getVerificationCode("86",ph);
                    checkCodeRow.setVisibility(View.VISIBLE);
                    reminderText();
                }
            }
        });

        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String user = username.getText().toString();
                String p1 = pass1.getText().toString();
                String p2 = pass2.getText().toString();
                String ph = phone.getText().toString();
                String em = email.getText().toString();
                String icode = checkcode.getText().toString();

                if (user.equals("")){
                    Toast.makeText(Register.this, "用户名不能为空", Toast.LENGTH_SHORT).show();
                }
                else if (p1.equals("")){
                    Toast.makeText(Register.this, "密码不能为空", Toast.LENGTH_SHORT).show();
                }
                else if (!p2.equals(p1)){
                    Toast.makeText(Register.this, "两次密码不一样", Toast.LENGTH_SHORT).show();
                }
                else if (ph.equals("")){
                    Toast.makeText(Register.this, "手机号不能为空", Toast.LENGTH_SHORT).show();
                }
                else if (em.equals("")){
                    Toast.makeText(Register.this, "邮箱为空将不利于您之后找回手机", Toast.LENGTH_SHORT).show();
                }
                else if (icode.equals("")){
                    Toast.makeText(Register.this, "验证码不能为空", Toast.LENGTH_SHORT).show();
                }
                else {

                    SMSSDK.submitVerificationCode("86", ph, icode);
                    flag = false;

                    if (validate){
                        Intent intent = new Intent(Register.this, login.class);
                        startActivity(intent);
                        finish();
                    }
                }
            }
        });

        cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Register.this, login.class);
                startActivity(intent);
                finish();
            }
        });
    }


    private void INIT(){
        submit = (Button) findViewById(R.id.submit);
        cancel = (Button) findViewById(R.id.cancel);
        check = (Button) findViewById(R.id.check);

        username = (EditText) findViewById(R.id.regisUser);
        pass1 = (EditText) findViewById(R.id.password1);
        pass2 = (EditText) findViewById(R.id.password2);
        phone = (EditText) findViewById(R.id.phone);
        email = (EditText) findViewById(R.id.email);
        checkcode = (EditText) findViewById(R.id.checkcode);

        checkCodeRow = (TableRow) findViewById(R.id.checkCodeRow);
        checkCodeRow.setVisibility(View.GONE);
    }


    //验证码送成功后提示文字

    private void reminderText() {
        handlerText.sendEmptyMessageDelayed(1, 1000);
    }

    Handler handlerText =new Handler(){
        public void handleMessage(Message msg) {
            if(msg.what==1){
                if(time>0){
                    check.setText(time + "秒");
                    time--;
                    handlerText.removeCallbacksAndMessages(null);
                    handlerText.sendEmptyMessageDelayed(1, 1000);
                }else{
                    check.setText("验证码");
                }
            }else{
                check.setText("验证码");
            }
        };
    };

    Handler handler=new Handler(){

        @Override
        public void handleMessage(Message msg) {
            // TODO Auto-generated method stub
            super.handleMessage(msg);
            int event = msg.arg1;
            int result = msg.arg2;
            Object data = msg.obj;
            Log.e("event", "event=" + event);
            if (result == SMSSDK.RESULT_COMPLETE) {

                //短信注册成功后，返回MainActivity,然后提示新好友
                if (event == SMSSDK.EVENT_SUBMIT_VERIFICATION_CODE) {//提交验证码成功,验证通过
                    Toast.makeText(getApplicationContext(), "验证码校验成功", Toast.LENGTH_SHORT).show();
                    handlerText.removeCallbacksAndMessages(null);
                    validate = true;
                } else if (event == SMSSDK.EVENT_GET_VERIFICATION_CODE){//服务器验证码发送成功
                    reminderText();
                    Toast.makeText(getApplicationContext(), "验证码已经发送", Toast.LENGTH_SHORT).show();
                    validate = false;
                }
            } else {
                if(flag){
                    Toast.makeText(Register.this, "验证码获取失败，请重新获取", Toast.LENGTH_SHORT).show();
                    phone.requestFocus();
                    validate = false;
                }else{
                    ((Throwable) data).printStackTrace();
                    Toast.makeText(Register.this, "验证码错误", Toast.LENGTH_SHORT).show();
                    validate = false;
                }
            }
        }
    };

}
