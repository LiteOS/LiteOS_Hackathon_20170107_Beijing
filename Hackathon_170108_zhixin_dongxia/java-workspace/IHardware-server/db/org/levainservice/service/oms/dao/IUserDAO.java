package org.levainservice.service.oms.dao;

import java.util.List;

import org.levainservice.service.oms.model.User;

public interface IUserDAO {
	public boolean insert(User user);

	public boolean update(User user);

	public boolean remove(User user);

	public User getUserById(long user);
	
	public User getUserByName(String userName);

	public List<User> queryUser(String userCNName);
	
	public boolean signin(String userName,String userPasswd);
}
