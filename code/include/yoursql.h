#pragma once  
#include <iostream>
#include <string>
#include "mysql.h"
#include <fstream>
#include "tools.h"
#include <unordered_set>
#include <stack>



class SQLconnectPool
{
public:
    SQLconnectPool(const char *HOST, const char *SQL_PORT, const char *USER, const char *PASSWORD, const char *TABLE);
    ~SQLconnectPool();
    MYSQL *applySQLConnect(bool &isbusy);
    void unapplySQLConnect(MYSQL *input);

private:
    std::unordered_set<MYSQL *> SQLconnectBusySet;
    std::stack<MYSQL *> SQLconnectPoolStack;
    int Connectsize = 8;
    const char *HOST;
    const char *USER;
    const char *PASSWORD;
    const char *TABLE;
    const char *SQL_PORT;
};
//--------------------------------------------------------------------------------
void SQLWrite(SQLconnectPool &pool, std::map<std::string, std::string> rec_c, bool &isbusy);
void SQLClear(SQLconnectPool &pool, bool &isbusy);
std::string SQLNewName(SQLconnectPool &pool, bool &isbusy);
void SQLBindName(SQLconnectPool &pool, bool &isbusy, long long createtime, std::string name);
void SQLGetData(SQLconnectPool &pool, std::string DATAname, std::string ESPname, std::string &receiveTimeLists, std::string &dataLists, bool &isbusy);
std::string GetstatusLeft(SQLconnectPool &pool, bool &isbusy, const char *SQLserverip, const char *SQLserverPort, const char *serverPort);
std::string GetstatusRight(SQLconnectPool &pool, bool &isbusy, const char *SQLserverip, const char *SQLserverPort, const char *serverPort);
std::string SQLGetNodeNameList(SQLconnectPool &pool, bool &isbusy);
