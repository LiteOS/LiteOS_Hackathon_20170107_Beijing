package com.example.apple.login;

import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class login extends AppCompatActivity {

    private static final String url = "http://10.1.18.7:5000/login";
    private static final int LOGIN_CONTENT = 0;

    private String user;
    private String pass;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        final EditText userEdit = (EditText) findViewById(R.id.username);
        final EditText passwordEdit = (EditText) findViewById(R.id.password);

        final Button login = (Button) findViewById(R.id.login);
        Button regist = (Button) findViewById(R.id.regist);

        Button forgetPassword = (Button) findViewById(R.id.forgetpassword);

        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                user = userEdit.getText().toString();
                pass = passwordEdit.getText().toString();
                if (user.equals("")) {
                    Toast.makeText(login.this, "用户名不能为空", Toast.LENGTH_SHORT).show();
                }
                else if (pass.equals("")){
                    Toast.makeText(login.this, "密码不能为空", Toast.LENGTH_SHORT).show();
                }
                else {
                    sendRequestWithHttpURLConnection(user, pass);
                }
            }
        });

        regist.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                Intent intent = new Intent(login.this, Register.class);
//                startActivity(intent);
//                finish();
            }
        });

        forgetPassword.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });

    }

    private void sendRequestWithHttpURLConnection(final String username, final String password){
        Thread t = new Thread(new Runnable(){
            @Override
            public void run(){
                HttpURLConnection connection = null;
                try{
                    connection = (HttpURLConnection)((new URL(url.toString())
                            .openConnection()));

                    // set method
                    connection.setRequestMethod("POST");
                    connection.setConnectTimeout(800000); //80s
                    connection.setReadTimeout(800000);//80s

                    DataOutputStream out = new DataOutputStream(
                            connection.getOutputStream());
                    //use post method to post our data
                    out.writeBytes("username=" + username
                            + "&password=" + password);


                    int code = connection.getResponseCode();

                    //get response data
                    InputStream in = connection.getInputStream();
                    BufferedReader reader = new BufferedReader(
                            new InputStreamReader(in));
                    StringBuilder response = new StringBuilder();

                    String line;
                    while((line = reader.readLine())!=null){
                        response.append(line);
                    }
                    Log.i("a",response.toString());

                    Message message = new Message();

                    message.what = LOGIN_CONTENT;
                    message.obj = response.toString();

                    handler.sendMessage(message);

                } catch(Exception e){
                    e.printStackTrace();
                } finally{
                    if(connection != null){
                        connection.disconnect();
                        handler.removeCallbacks(this);
                    }
                }
            }
        });
        t.start();
    }

    private Handler handler = new Handler(){
        public void handleMessage(Message message){
            switch(message.what){
                case LOGIN_CONTENT:
                    if(message.obj.equals("0")){
                        Toast.makeText(login.this, "用户名和密码不对应", Toast.LENGTH_SHORT).show();
                    }
                    else {
                        Intent intent = new Intent(login.this, DevicesShow.class);
                        Bundle bundle = new Bundle();
                        bundle.putString("username", user);
                        intent.putExtras(bundle);
                        startActivity(intent);
                        finish();
                    }
                    break;
                default:
                    break;
            }
        }
    };
}
