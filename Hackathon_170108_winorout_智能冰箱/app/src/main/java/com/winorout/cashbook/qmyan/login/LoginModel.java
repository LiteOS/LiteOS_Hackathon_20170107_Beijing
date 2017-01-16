package com.winorout.cashbook.qmyan.login;

import android.content.Context;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import static com.winorout.cashbook.common.MyApplication.dbHelper;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class LoginModel implements ILoginModel {

    private Context mContext;

    public LoginModel(Context context) {
        mContext = context;
    }

    @Override
    public void login(String userName, String userPassword, OnLoginListener listener) {
        SQLiteDatabase db = dbHelper.getReadableDatabase();
        Cursor result = db.query("User", new String[]{"userPassword"}, "userName = ?", new String[]{userName}, null, null, null);
        if (result.moveToFirst()) {
            String password = result.getString(result.getColumnIndex("userPassword"));
            if (password.equals(userPassword)) {
                SharedPreferences.Editor editor = mContext.getSharedPreferences("loginUser", Context.MODE_PRIVATE).edit();
                editor.putBoolean("isLogin", true);
                editor.putString("userName", userName);
                editor.putString("userPassword", userPassword);
                editor.commit();
                listener.onSuccess();
            } else {
                listener.onFailed("密码输入错误");
            }
        } else {
            listener.onFailed("用户不存在");
        }
        if (db != null) {
            db.close();
        }
    }

}
