package Bean;

import javax.ejb.Remote;
import javax.ejb.Stateful;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import Interface.HomeBeanRemote;
import Model.User;

/**
 * Session Bean implementation class BeanHome
 */
@Stateful
@Remote(HomeBeanRemote.class)
public class HomeBean implements HomeBeanRemote {

	@PersistenceContext
	EntityManager em;
	
    /**
     * Default constructor. 
     */
    public HomeBean() {
        // TODO Auto-generated constructor stub
    }

	@Override
	public String test() {
		// TODO Auto-generated method stub
		return "\"yay\"";
	}

	@Override
	public void createUser(String email, String pw) {
		// TODO Auto-generated method stub
		User u = new User(email,pw);
		em.persist(u);
		em.flush();
	}

}
