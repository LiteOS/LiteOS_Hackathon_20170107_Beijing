package com.pktech.cooperation.service.model;

import java.util.Date;

import com.google.gson.Gson;

public class BusinessTrip extends User {
	private static final long serialVersionUID = -8981862719623506756L;
	private String businessTripId;
	private String customName;
	private String businessTripReason;
	private Date businessTripDate;
	private String transportMeans;
	private String number;

	public String getBusinessTripId() {
		return businessTripId;
	}

	public void setBusinessTripId(String businessTripId) {
		this.businessTripId = businessTripId;
	}

	public String getCustomName() {
		return customName;
	}

	public void setCustomName(String customName) {
		this.customName = customName;
	}

	public String getBusinessTripReason() {
		return businessTripReason;
	}

	public void setBusinessTripReason(String businessTripReason) {
		this.businessTripReason = businessTripReason;
	}

	public Date getBusinessTripDate() {
		return businessTripDate;
	}

	public void setBusinessTripDate(Date businessTripDate) {
		this.businessTripDate = businessTripDate;
	}

	public String getTransportMeans() {
		return transportMeans;
	}

	public void setTransportMeans(String transportMeans) {
		this.transportMeans = transportMeans;
	}

	public String getNumber() {
		return number;
	}

	public void setNumber(String number) {
		this.number = number;
	}

	public static void main(String[] args){
		BusinessTrip trip = new BusinessTrip();
		trip.setUserId("13348613030");
		trip.setBusinessTripReason("湖南电信出差");
		trip.setBusinessTripId("1034589");
		trip.setBusinessTripDate(new Date());
		trip.setCustomName("湖南电信");
		trip.setTransportMeans("火车");
		trip.setNumber("G80");
	
		System.out.println(new Gson().toJson(trip));
	}
}
