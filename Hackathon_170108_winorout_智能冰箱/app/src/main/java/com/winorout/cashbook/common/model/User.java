package com.winorout.cashbook.common.model;

/**
 * Created by micheal-yan on 2016/12/2.
 */

public class User {

    private int id;
    private String userName;		//用户名
    private String userPassword;	//密码
    private String email;       	//密保邮箱
    private byte[] userPhoto;		//用户头像
    private double totalAssets;		//总资产
    private double generalIncome;	//总收入
    private double overallCost;		//总支出

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getUserPassword() {
        return userPassword;
    }

    public void setUserPassword(String userPassword) {
        this.userPassword = userPassword;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public byte[] getUserPhoto() {
        return userPhoto;
    }

    public void setUserPhoto(byte[] userPhoto) {
        this.userPhoto = userPhoto;
    }

    public double getTotalAssets() {
        return totalAssets;
    }

    public void setTotalAssets(double totalAssets) {
        this.totalAssets = totalAssets;
    }

    public double getGeneralIncome() {
        return generalIncome;
    }

    public void setGeneralIncome(double generalIncome) {
        this.generalIncome = generalIncome;
    }

    public double getOverallCost() {
        return overallCost;
    }

    public void setOverallCost(double overallCost) {
        this.overallCost = overallCost;
    }
}
