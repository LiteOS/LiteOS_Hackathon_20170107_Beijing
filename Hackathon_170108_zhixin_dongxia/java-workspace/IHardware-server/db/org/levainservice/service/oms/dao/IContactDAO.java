package org.levainservice.service.oms.dao;

import java.util.List;

import org.levainservice.service.oms.model.Contact;

public interface IContactDAO {
	public boolean insert(Contact device);

	public boolean update(Contact device);

	public boolean remove(Contact device);

	public Contact getContactById(long deviceId);

	public List<Contact> getContactsBySerialNo(String deviceSerialNo);

}
