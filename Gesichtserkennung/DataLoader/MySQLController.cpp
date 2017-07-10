/**
 * @file MySQLController.cpp
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 26.06.2017
 * @brief Class file of MySQLController class.
 *
 */

#include "MySQLController.h"

/**
 * \brief Get users from database.
 *
 * This function creates a database connection according to the given database information.
 * \param[in] host Enter hostname of database here.
 * \param[in] databaseUser Enter username of database here.
 * \param[in] password Enter password of user here.
 * \return a vector of \See User.
 */
std::vector<User*> MySQLController::getUsersFromDatabase(std::string host, std::string databaseUser, std::string password){
    std::vector<User*> users;
    User* user;

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res_user, *res_foto;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(host, databaseUser, password);
        /* Connect to the MySQL test database */
        con->setSchema("test2");

        stmt = con->createStatement();
        res_user = stmt->executeQuery("SELECT * from user");
        while (res_user->next()) {
            user = new User(res_user->getInt("id"), res_user->getString("name"));
            res_foto = stmt->executeQuery("SELECT * from foto where user_id = " + std::to_string(res_user->getInt("id")));
            while(res_foto->next()){
                //user->addImage(res_foto->getBlob("bild"));
            }
            users.push_back(user);
        }
        delete res_user;
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }

    return users;
}

