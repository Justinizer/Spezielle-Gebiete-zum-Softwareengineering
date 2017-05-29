package Interface;

import javax.ejb.Remote;

@Remote
public interface HomeBeanRemote {
	public String test();

	public void createUser(String email, String pw);
}
