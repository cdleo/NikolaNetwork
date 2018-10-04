/* 
 * File:   DatabaseManager.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:44 PM
 */


#ifndef DATABASEMANAGER_H
#define	DATABASEMANAGER_H

#include <db/MySQLConnector.h>
#include <synapse/GlobalDefinitions.h>

#define DB DatabaseManager::getInstance()

using namespace Synapse;

class DatabaseManager : public SingletonBase<DatabaseManager> {
public:
    friend class SingletonBase<DatabaseManager>;
    
    bool connect();
    void disconnect();
    bool isConnected();

    ValidationResult loadUser(std::string user, std::string pwd, int& securityLeve);
    
private:
    DatabaseManager();
    virtual ~DatabaseManager() throw();
};

#endif	/* DATABASEMANAGER_H */


