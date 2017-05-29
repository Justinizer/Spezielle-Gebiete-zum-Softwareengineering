package Model;

import java.io.Serializable;
import java.util.List;

import javax.persistence.*;

/**
 * Entity implementation class for Entity: User
 *
 */
@Entity

public class User implements Serializable {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Integer id;
	
	private String password;
	private String email;
	
	@ManyToMany
	@JoinTable(
		      name="User_Thing",
		      joinColumns=@JoinColumn(name="User_id", referencedColumnName="ID"),
		      inverseJoinColumns=@JoinColumn(name="Thing_id", referencedColumnName="ID"))
	private List<Thing> allowedToUse;
	
	private static final long serialVersionUID = 1L;

	/**
	 * Required for JPA
	 */
	public User(){
		super();
	}
	
	public User(String email, String password) {
		super();
		this.password = password;
		this.email= email;
	}
   
}
