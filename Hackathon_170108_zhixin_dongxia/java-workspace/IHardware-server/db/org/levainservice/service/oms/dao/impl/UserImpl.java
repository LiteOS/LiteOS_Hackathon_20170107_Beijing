package org.levainservice.service.oms.dao.impl;

import java.util.List;

import javax.persistence.EntityManager;

import org.levainservice.service.db.util.EntityManagerUtil;
import org.levainservice.service.oms.dao.IUserDAO;
import org.levainservice.service.oms.model.User;

public class UserImpl implements IUserDAO {
	EntityManager em = EntityManagerUtil.getEntityManager();

	@Override
	public boolean insert(User user) {
		boolean result = true;
		em.getTransaction().begin();
		em.persist(user);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public boolean update(User user) {
		boolean result = true;
		em.getTransaction().begin();
		em.merge(user);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public boolean remove(User user) {
		boolean result = true;
		em.getTransaction().begin();
		User user1 = this.getUserById(user.getUserId());
		em.remove(user1);
		em.getTransaction().commit();
		em.close();
		return result;
	}

	@Override
	public User getUserById(long userId) {
		em.getTransaction().begin();
		// 查找id为1的User,进行更新
		User user = em.find(User.class, userId);// User 的主键id为Long型

		em.getTransaction().commit();
		em.close();
		return user;
	}

	@Override
	public User getUserByName(String userName) {
		User user = (User) em
				.createQuery("SELECT u FROM User u WHERE u.userName=:userName")
				.setParameter("userName", userName).getSingleResult();
		return user;
	}

	@Override
	public List<User> queryUser(String userCNName) {
		@SuppressWarnings("unchecked")
		List<User> users = (List<User>) em
				.createQuery(
						"SELECT u FROM User u where u.userCNName like :cnName")
				.setParameter("cnName", "%" + userCNName + "%")
				.getResultList();
		return users;
	}

	@Override
	public boolean signin(String userName, String userPasswd) {
		EntityManager em = EntityManagerUtil.getEntityManager();

		User user = (User) em
				.createQuery("SELECT u FROM User u WHERE u.userName=:userName")
				.setParameter("userName", userName).getSingleResult();
		return userPasswd.equals(user.getUserPasswd());
	}

}
