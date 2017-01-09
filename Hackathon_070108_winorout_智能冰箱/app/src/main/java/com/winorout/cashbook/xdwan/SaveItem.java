package com.winorout.cashbook.xdwan;

/**
 * Created by Mr-x on 2016/12/09.
 */

public class SaveItem {
    public int imageView;
    public String textView;
    public Double money;

    public void setTextView(String textView) {
        this.textView = textView;
    }

    public void setMoney(Double money) {
        this.money = money;
    }

    public void setImageView(int imageView) {
        this.imageView = imageView;
    }

    public String getTextView() {
        return textView;
    }

    public int getImageView() {
        return imageView;
    }

    public Double getMoney() {
        return money;
    }
}
