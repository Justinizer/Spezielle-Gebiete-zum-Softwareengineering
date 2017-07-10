/**
 * @file MySQLController.h
 * @Author Justin Jagieniak, Gamze Söylev Öktem
 * @date 26.06.2017
 * @brief Header file of MySQLController class.
 *
 */

#ifndef GESICHTSERKENNUNG_MYSQLCONTROLLER_H
#define GESICHTSERKENNUNG_MYSQLCONTROLLER_H

#include "../Models/User.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>

/**
 * \brief A class to connect to a mysql database.
 *
 * This class connects to a mysql database and converts the gathered datas to the objects of the specific model class.
 */
class MySQLController {
public:
    std::vector<User*> getUsersFromDatabase(std::string, std::string, std::string);

};


#endif //GESICHTSERKENNUNG_MYSQLCONTROLLER_H
