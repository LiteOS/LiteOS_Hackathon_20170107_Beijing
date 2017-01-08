package org.levainservice.service.oms.dao;

import java.util.List;

import org.levainservice.service.oms.model.Device;

public interface IDeviceDAO {
	public boolean insert(Device device);

	public boolean update(Device device);

	public boolean remove(Device device);

	public Device getDeviceById(long userId);
	
	public Device getDeviceBySerialNo(String serialNo);

	public List<Device> getDevicesByUser(long userId);

}
