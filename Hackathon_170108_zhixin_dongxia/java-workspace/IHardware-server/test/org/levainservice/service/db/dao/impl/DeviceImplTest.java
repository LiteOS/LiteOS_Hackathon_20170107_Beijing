package org.levainservice.service.db.dao.impl;

import static org.junit.Assert.fail;

import java.util.List;

import junit.framework.Assert;

import org.junit.Test;
import org.levainservice.service.oms.dao.IDeviceDAO;
import org.levainservice.service.oms.dao.impl.DeviceImpl;
import org.levainservice.service.oms.model.Device;

public class DeviceImplTest {
	IDeviceDAO dao = new DeviceImpl();
	
	@Test
	public void testInsert() {
		Device d = new Device();
		d.setDeviceId(2L);
		d.setDeviceSerialNo("OMS-0002");
		d.setDevicePos("荷花园电信大厦3306");
		d.setUserId(1L);
		
		dao.insert(d);
	}

	@Test
	public void testUpdate() {
		fail("Not yet implemented");
	}

	@Test
	public void testRemove() {
		fail("Not yet implemented");
	}

	@Test
	public void testGetDeviceById() {
		fail("Not yet implemented");
	}

	@Test
	public void testGetDeviceBySerialNo() {
		Device d = dao.getDeviceBySerialNo("OMS-0001");
		Assert.assertEquals("荷花园电信大厦3305", d.getDevicePos());
	}

	@Test
	public void testGetDevicesByUser() {
		List<Device> ds = dao.getDevicesByUser(1L);
		for(int i=0;i<ds.size();i++){
			System.out.println(ds.get(i).getDevicePos());
		}
	}

}
