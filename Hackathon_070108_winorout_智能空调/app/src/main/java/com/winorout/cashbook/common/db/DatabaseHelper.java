package com.winorout.cashbook.common.db;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

/**
 * Created by micheal-yan on 2016/12/2.
 */

public class DatabaseHelper extends SQLiteOpenHelper {

    /**
     * 创建用户表语句
     */
    public static final String CREATE_USER = "create table User ("
            + "id integer primary key autoincrement,"
            + "userName text UNIQUE,"
            + "userPassword text,"
            + "userPhoto blob,"
            + "email text,"
            + "totalAssets real,"
            + "generalIncome real,"
            + "overallCost real)";

    /**
     * 创建财务表语句
     */
    public static final String CREATE_FINACE = "create table Finace ("
            + "id integer primary key autoincrement,"
            + "userId integer,"
            + "categoryType text,"
            + "categoryName text,"
            + "typePic int,"
            + "amount text,"
            + "date text,"
            + "note text)";

    public DatabaseHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(CREATE_USER);
        db.execSQL(CREATE_FINACE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }
}
