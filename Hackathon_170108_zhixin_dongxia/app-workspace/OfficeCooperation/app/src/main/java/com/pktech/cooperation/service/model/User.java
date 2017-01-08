package com.pktech.cooperation.service.model;

public class User implements java.io.Serializable {
	private static final long serialVersionUID = -6115967818769232895L;
	
	private String userId;
	private String userName;
	private String passwd;

	public String getUserId() {
		return userId;
	}

	public void setUserId(String userId) {
		this.userId = userId;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public String getPasswd() {
		return passwd;
	}

	public void setPasswd(String passwd) {
		this.passwd = passwd;
	}

}
