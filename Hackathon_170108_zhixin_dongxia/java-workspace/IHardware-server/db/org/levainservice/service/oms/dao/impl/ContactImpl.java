package org.levainservice.service.oms.dao.impl;

import java.util.List;

import javax.persistence.EntityManager;

import org.levainservice.service.db.util.EntityManagerUtil;
import org.levainservice.service.oms.dao.IContactDAO;
import org.levainservice.service.oms.dao.IDeviceDAO;
import org.levainservice.service.oms.model.Contact;
import org.levainservice.service.oms.model.Device;

public class ContactImpl implements IContactDAO {
	EntityManager em = EntityManagerUtil.getEntityManager();

	@Override
	public boolean insert(Contact device) {
		boolean result = true;
		em.getTransaction().begin();
		em.persist(device);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public boolean update(Contact device) {
		boolean result = true;
		em.getTransaction().begin();
		em.merge(device);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public boolean remove(Contact device) {
		boolean result = true;
		em.getTransaction().begin();
		Contact device1 = this.getContactById(device.getContactId());
		em.remove(device1);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public Contact getContactById(long deviceId) {
		em.getTransaction().begin();
		Contact device = em.find(Contact.class, deviceId);// Contact 的主键id为Long型
		em.getTransaction().commit();
		em.close();
		return device;
	}

	@Override
	public List<Contact> getContactsBySerialNo(String deviceSerialNo) {
		IDeviceDAO dao = new DeviceImpl();
		Device device = dao.getDeviceBySerialNo(deviceSerialNo);
		@SuppressWarnings("unchecked")
		List<Contact> devices = (List<Contact>) em
				.createQuery(
						"SELECT c FROM Contact c where c.deviceId=:deviceId")
				.setParameter("deviceId", device.getDeviceId()).getResultList();
		return devices;
	}

}
