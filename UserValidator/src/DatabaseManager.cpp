/* 
 * File:   DatabaseManager.cpp
 * Author: cleo
 * 
 * Created on July 29, 2014, 3:44 PM
 */

#include <DatabaseManager.h>
#include <Parameters.h>

#include <exceptions/DBExceptions.h>


DatabaseManager::DatabaseManager() {  
}
// =============================================================================

DatabaseManager::~DatabaseManager() throw() {
    if (this->isConnected())
        this->disconnect();
    
    MySQLConnector::destroyInstance();    
}
// =============================================================================

bool DatabaseManager::connect(){
    
    if (this->isConnected()) this->disconnect();
    
    std::string dbName, dbHost, dbUsr, dbPwd;
    PARAMETERS->getValue("dbName", dbName);
    PARAMETERS->getValue("dbHost", dbHost);
    PARAMETERS->getValue("dbUsr", dbUsr);
    PARAMETERS->getValue("dbPwd", dbPwd);
    dbConnector->setConnectionParameters( dbName, dbHost, dbUsr, dbPwd);    
    
    return dbConnector->connect();
}
// =============================================================================

void DatabaseManager::disconnect(){
    dbConnector->disconnect();
}
// =============================================================================

bool DatabaseManager::isConnected(){
    return dbConnector->isConnected();
}
// =============================================================================

ValidationResult DatabaseManager::loadUser(std::string user, std::string pwd, int& securityLevel) {
    
    std::stringstream sql;
    sql << "SELECT Pwd, Level "
        << "  FROM Nikola.Users, Nikola.SecurityLevels "
        << " WHERE User='" << user << "'"
        << "   AND idSecurityLevels = securityLevelsId";
    
    mysqlpp::StoreQueryResult queryResult;
    bool result = dbConnector->queryData( sql.str(), queryResult );
    if (!result)
        return ValidationResult_GenericError;
    
    if (queryResult.empty())
        return ValidationResult_UnknownUser;
        
    mysqlpp::Row row = queryResult[0];
    if (row["Pwd"].compare( pwd ) != 0)
        return ValidationResult_InvalidPassword;
    
    securityLevel = atoi( row["Level"].c_str() );
    return ValidationResult_Ok;

}
// =============================================================================
