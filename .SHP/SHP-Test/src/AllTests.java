import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ TestRestAutomation.class, TestRestThing.class, TestRestUser.class })
public class AllTests {

}
