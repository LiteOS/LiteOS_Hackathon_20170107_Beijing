package com.hiwhitley.smartswitch.entity;

/**
 * Created by hiwhitley on 2017/1/8.
 */

public class User {
    private String userID;
    private String tel;
    private String password;
    private String nickName;
    private int status;
    private String msg;

    public String getTel() {
        return tel;
    }

    public void setTel(String tel) {
        this.tel = tel;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public String getUserID() {
        return userID;
    }

    public void setUserID(String userID) {
        this.userID = userID;
    }

    @Override
    public String toString() {
        return "User{" +
                "userID='" + userID + '\'' +
                ", tel='" + tel + '\'' +
                ", password='" + password + '\'' +
                ", nickName='" + nickName + '\'' +
                ", status=" + status +
                ", msg='" + msg + '\'' +
                '}';
    }

    public boolean isSuccessful() {
        return status == 1;
    }
}
