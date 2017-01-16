package com.winorout.cashbook.qmyan.register;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import static com.winorout.cashbook.common.MyApplication.dbHelper;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class RegisterModel implements IRegisterModel {

    @Override
    public void register(String userName, String userPassword, String email, OnRegisterListener listener) {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        Cursor result = db.query("User", null, "userName = ?", new String[]{userName}, null, null, null);
        if (result.moveToFirst()) {
            listener.onFailed("用户已存在");
        } else {
            ContentValues values = new ContentValues();
            values.put("userName", userName);
            values.put("userPassword", userPassword);
            values.put("email", email);
            long id = db.insert("User", null, values);
            if (id >0) {
                listener.onSuccess();
            } else {
                listener.onFailed("创建账户失败");
            }
        }
        if (db != null) {
            db.close();
        }
    }
}
