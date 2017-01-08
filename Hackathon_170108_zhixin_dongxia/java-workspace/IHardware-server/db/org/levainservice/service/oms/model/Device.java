package org.levainservice.service.oms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "Devices")
public class Device implements java.io.Serializable {
	private static final long serialVersionUID = 1L;
	@Id
	@Column(name = "deviceId")
	private long deviceId;
	@Column(name = "devicePos")
	private String devicePos;
	@Column(name = "deviceSerialNo")
	private String deviceSerialNo;

	@Column(name = "userId")
	private long userId;

	public long getDeviceId() {
		return deviceId;
	}

	public void setDeviceId(long deviceId) {
		this.deviceId = deviceId;
	}

	public String getDevicePos() {
		return devicePos;
	}

	public void setDevicePos(String devicePos) {
		this.devicePos = devicePos;
	}

	public String getDeviceSerialNo() {
		return deviceSerialNo;
	}

	public void setDeviceSerialNo(String deviceSerialNo) {
		this.deviceSerialNo = deviceSerialNo;
	}

	public long getUserId() {
		return userId;
	}

	public void setUserId(long userId) {
		this.userId = userId;
	}

}
