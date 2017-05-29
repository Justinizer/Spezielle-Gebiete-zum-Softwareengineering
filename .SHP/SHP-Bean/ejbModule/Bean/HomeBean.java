package Bean;

import java.util.List;

import javax.ejb.Remote;
import javax.ejb.Stateful;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import Interface.HomeBeanRemote;
import Model.Thing;
import Model.User;

/**
 * Session Bean implementation class BeanHome
 */
@Stateful
@Remote(HomeBeanRemote.class)
public class HomeBean implements HomeBeanRemote {

	@PersistenceContext
	EntityManager em;

	private boolean isLoggedin = false;
	private User user;

	/**
	 * Default constructor.
	 */
	public HomeBean() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public String test() {
		return "\"yay\"";
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#createUser(java.lang.String,
	 * java.lang.String)
	 */
	@Override
	public boolean createUser(String email, String pw) {
		if (pw == null) {
			return false;
		}
		if (email == null) {
			return false;
		}
		if (!email.contains("@")) {
			return false;
		}
		if (pw.length() < 2) {
			return false;
		}
		/* parameter are valid, create an persist a new user */
		User u = new User(email, pw);
		em.persist(u);
		em.flush();
		return true;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see Interface.HomeBeanRemote#checkLogin(java.lang.String,
	 * java.lang.String)
	 */
	@Override
	public boolean checkLogin(String email, String pw) {
		if (pw == null) {
			return false;
		}
		if (email == null) {
			return false;
		}

		try {
			User result = em.createNamedQuery(User.LOGIN, User.class).setParameter("email", email)
					.setParameter("password", pw).getSingleResult();
			if (result != null) {
				isLoggedin = true;
				user = result;
				return true;
			}
		} catch (Exception e) {
		}
		return false;
	}

	@Override
	@SuppressWarnings("unchecked")
	public List<Thing> getAllThings() {
		if (isLoggedin) {
			try {
				return em.createNamedQuery(Thing.GET_ALL_THINGS).getResultList();
			} catch (Exception e) {

			}
		}
		return null;
	}

}
