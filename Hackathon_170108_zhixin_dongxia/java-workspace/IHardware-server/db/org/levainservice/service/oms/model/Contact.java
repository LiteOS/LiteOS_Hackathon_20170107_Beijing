package org.levainservice.service.oms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "Contacts")
public class Contact implements java.io.Serializable {
	private static final long serialVersionUID = 1L;
	@Id
	@Column(name = "contactId")
	private long contactId;
	@Column(name = "contactName")
	private String contactName;
	@Column(name = "mobile")
	private String mobile;
	@Column(name = "tel")
	private String tel;

	@Column(name = "deviceId")
	private long deviceId;

	public long getContactId() {
		return contactId;
	}

	public void setContactId(long contactId) {
		this.contactId = contactId;
	}

	public String getContactName() {
		return contactName;
	}

	public void setContactName(String contactName) {
		this.contactName = contactName;
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

	public long getDeviceId() {
		return deviceId;
	}

	public void setDeviceId(long deviceId) {
		this.deviceId = deviceId;
	}

}
