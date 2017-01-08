package org.levainservice.service.oms.dao.impl;

import java.util.List;

import javax.persistence.EntityManager;

import org.levainservice.service.db.util.EntityManagerUtil;
import org.levainservice.service.oms.dao.IDeviceDAO;
import org.levainservice.service.oms.model.Device;

public class DeviceImpl implements IDeviceDAO {
	EntityManager em = EntityManagerUtil.getEntityManager();

	@Override
	public boolean insert(Device device) {
		boolean result = true;
		em.getTransaction().begin();
		em.persist(device);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public boolean update(Device device) {
		boolean result = true;
		em.getTransaction().begin();
		em.merge(device);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public boolean remove(Device device) {
		boolean result = true;
		em.getTransaction().begin();
		Device device1 = this.getDeviceById(device.getDeviceId());
		em.remove(device1);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public Device getDeviceById(long deviceId) {
		em.getTransaction().begin();
		// 查找id为1的Device,进行更新
		Device device = em.find(Device.class, deviceId);// Device 的主键id为Long型

		em.getTransaction().commit();
		em.close();
		return device;
	}

	@Override
	public Device getDeviceBySerialNo(String serialNo) {
		Device device = (Device) em
				.createQuery(
						"SELECT d FROM Device d WHERE d.deviceSerialNo=:serialNo")
				.setParameter("serialNo", serialNo).getSingleResult();
		return device;
	}

	@Override
	public List<Device> getDevicesByUser(long userId) {
		@SuppressWarnings("unchecked")
		List<Device> devices = (List<Device>) em
				.createQuery(
						"SELECT d FROM Device d where d.userId=:userId")
				.setParameter("userId", userId)
				.getResultList();
		return devices;
	}

}
