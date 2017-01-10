package com.winorout.cashbook.qmyan.changepwd;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import static com.winorout.cashbook.common.MyApplication.dbHelper;

/**
 * Created by micheal-yan on 2016/12/9.
 */

public class ChangepwdModel implements IChangepwdModel {

    @Override
    public void changePwd(String userPassword, String email, OnChangePwdListener listener) {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        Cursor result = db.query("User", new String[]{"userName"}, "email = ?", new String[]{email}, null, null, null);
        if (result.moveToFirst()) {
            ContentValues values = new ContentValues();
            values.put("userPassword", userPassword);
            int number = db.update("User", values, "email = ?", new String[]{email});
            if (number > 0) {
                listener.onSuccess();
            } else {
                listener.onFailed("修改密码失败");
            }
        } else {
            listener.onFailed("请输入正确的密保邮箱");
        }
        if (db != null) {
            db.close();
        }
    }
}
