package org.levainservice.service.db.dao.impl;

import static org.junit.Assert.fail;

import java.util.List;

import junit.framework.Assert;

import org.junit.Test;
import org.levainservice.service.oms.dao.IUserDAO;
import org.levainservice.service.oms.dao.impl.UserImpl;
import org.levainservice.service.oms.model.User;

@SuppressWarnings("deprecation")
public class UserImplTest {
	IUserDAO dao = new UserImpl();
	
	@Test
	public void testInsert() {
//		User user = new User();
//		user.setUserId(1L);
//		user.setUserName("test");
//		user.setUserCNName("Фݼ");
//		user.setUserPasswd("123456");
//		user.setMobile("13348610001");
//		user.setTel("0731-8488884");
//		
//		dao.insert(user);
	}

	@Test
	public void testUpdate() {
//		User user = new User();
//		user.setUserId(1L);
//		user.setUserName("test2");
//		user.setUserCNName("Фݼ2");
//		user.setUserPasswd("1234562");
//		user.setMobile("13348610002");
//		user.setTel("0731-8488882");
//		
//		dao.update(user);
	}

	@Test
	public void testRemove() {
		fail("Not yet implemented");
	}

	@Test
	public void testGetByUserId() {
//		User user = dao.getByUserId(1);
//		System.out.println(user.getUserCNName());
	}

	@Test
	public void testGetByUserName() {
//		User user = dao.getByUserName("test2");
//		System.out.println(user.getUserCNName());
	}

	@Test
	public void testQueryUser() {
		List<User> users = dao.queryUser("��");
		System.out.println(users.get(0).getUserCNName());
	}
	
	@Test
	public void testSignin() {
		boolean result = dao.signin("test2", "1234562");
		Assert.assertTrue(result);
	}

}
