package org.levainservice.service.oms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "Users")
public class User implements java.io.Serializable {
	private static final long serialVersionUID = 1L;
	@Id
	@Column(name = "userId")
	private long userId;
	@Column(name = "userName")
	private String userName;
	@Column(name = "userPasswd")
	private String userPasswd;
	@Column(name = "userCNName")
	private String userCNName;
	@Column(name = "mobile")
	private String mobile;
	@Column(name = "tel")
	private String tel;

	public long getUserId() {
		return userId;
	}

	public void setUserId(long userId) {
		this.userId = userId;
	}

	public String getUserCNName() {
		return userCNName;
	}

	public void setUserCNName(String userCNName) {
		this.userCNName = userCNName;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public String getUserPasswd() {
		return userPasswd;
	}

	public void setUserPasswd(String userPasswd) {
		this.userPasswd = userPasswd;
	}

	public String getMobile() {
		return mobile;
	}

	public void setMobile(String mobile) {
		this.mobile = mobile;
	}

	public String getTel() {
		return tel;
	}

	public void setTel(String tel) {
		this.tel = tel;
	}

}
