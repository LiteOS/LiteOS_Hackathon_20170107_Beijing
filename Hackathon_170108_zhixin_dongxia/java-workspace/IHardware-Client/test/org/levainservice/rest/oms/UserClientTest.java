package org.levainservice.rest.oms;

import org.junit.Test;
import org.levainservice.rest.Response;
import org.levainservice.service.oms.model.User;

import com.google.gson.Gson;

public class UserClientTest {
	UserClient client = new UserClient();
	
	@Test
	public void testInsert() {
//		User user = new User();
//		user.setUserName("rest-001");
//		user.setUserPasswd("rest-p-001");
//		user.setMobile("15300010001");
//		user.setTel("0731-88888881");
//		
//		Response r = client.insert(user);
//		System.out.println(new Gson().toJson(r));
	}

	@Test
	public void testSignin() {
		User user = new User();
		user.setUserName("rest-001");
		user.setUserPasswd("rest-p-001");
		
		Response r = client.signin(user);
		System.out.println(new Gson().toJson(r));
	}

}
