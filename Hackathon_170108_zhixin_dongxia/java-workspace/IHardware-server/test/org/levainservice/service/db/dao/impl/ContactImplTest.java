package org.levainservice.service.db.dao.impl;

import static org.junit.Assert.fail;

import java.util.List;

import org.junit.Test;
import org.levainservice.service.oms.dao.IContactDAO;
import org.levainservice.service.oms.dao.impl.ContactImpl;
import org.levainservice.service.oms.model.Contact;

public class ContactImplTest {
	IContactDAO dao = new ContactImpl();
	@Test
	public void testInsert() {
		Contact c = new Contact();
		c.setContactId(2L);
		c.setContactName("Àî¹ú");
		c.setDeviceId(1L);
		c.setMobile("13348610004");
		c.setTel("0731-85667834");
		
		dao.insert(c);
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
	public void testGetContactById() {
		fail("Not yet implemented");
	}

	@Test
	public void testGetContactsBySerialNo() {
		List<Contact> contacts = dao.getContactsBySerialNo("OMS-0001");
		
		for(int i=0;i<contacts.size();i++){
			System.out.println(contacts.get(i).getMobile());
		}
	}

}
