package com.hiwhitley.smartswitch.entity;

/**
 * Created by hiwhitley on 2017/1/7.
 */

public class ResponseBody {


    /**
     * status : 1
     * msg : 成功
     */

    private int status;
    private String msg;

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

    public boolean isSuccessful() {
        return status == 1;
    }

    @Override
    public String toString() {
        return "ResponseBody{" +
                "status=" + status +
                ", msg='" + msg + '\'' +
                '}';
    }
}
