package com.winorout.cashbook.common;

import android.app.Application;
import android.content.ContentValues;
import android.content.Context;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import com.winorout.cashbook.common.db.DatabaseHelper;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class MyApplication extends Application {

    //数据库名
    public static final String DATABASE_NAME = "CashBook.db";
    public static DatabaseHelper dbHelper;

    @Override
    public void onCreate() {
        super.onCreate();
        dbHelper = new DatabaseHelper(this, DATABASE_NAME, null, 1);
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put("userName", "default");
        values.put("userPassword", "1000");
        int id = (int) db.insert("User", null, values);
        Log.d("TAG", "default user id : " + id);
        if (id > 0) {
            SharedPreferences.Editor editor = getSharedPreferences("loginUser", Context.MODE_PRIVATE).edit();
            editor.putString("userName", "default");
            editor.putInt("userId", id);
            editor.commit();
        }
        if (db != null) {
            db.close();
        }
    }
}
