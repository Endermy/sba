#include "yoursql.h"
#include <iostream>
#include <string>
#include <map>
#include "mysql.h"
#include "tools.h"
#include <stack>

SQLconnectPool::SQLconnectPool(const char *HOST, const char *SQL_PORT, const char *USER, const char *PASSWORD, const char *TABLE)
    : HOST(HOST), SQL_PORT(SQL_PORT), USER(USER), PASSWORD(PASSWORD), TABLE(TABLE)
{
    while (SQLconnectPoolStack.size() < Connectsize)
    {
        SQLconnectPoolStack.push(mysql_init(NULL));
        mysql_real_connect(SQLconnectPoolStack.top(), HOST, USER, PASSWORD, TABLE, cti(SQL_PORT), NULL, 0);

        mysql_set_character_set(SQLconnectPoolStack.top(), "GBK");
    }
}
SQLconnectPool::~SQLconnectPool()
{
    std::cout << "[prerelease]" << "busy number:" << SQLconnectBusySet.size() << "|pool number:" << SQLconnectPoolStack.size() << std::endl;
    while (!SQLconnectBusySet.empty())
    {
        std::cout << "can not exit! " << SQLconnectBusySet.size() << " link busying" << std::endl;
    }
    while (SQLconnectPoolStack.size() != 0)
    {
        std::cout << "[releasing]" << SQLconnectPoolStack.size() << " left" << std::endl;
        mysql_close(SQLconnectPoolStack.top());
        SQLconnectPoolStack.pop();
    }
    std::cout << "[released]" << std::endl;
}

MYSQL *SQLconnectPool::applySQLConnect(bool &isbusy)
{
    MYSQL *_return;
    bool finished = false;
    while (!finished)
    {
        if (!isbusy)
        {
            isbusy = true;
            if (SQLconnectPoolStack.size() <= Connectsize / 2)
            {
                Connectsize *= 2;
                std::cout << Connectsize << std::endl;
                while (SQLconnectPoolStack.size() < Connectsize)
                {
                    SQLconnectPoolStack.push(mysql_init(NULL));
                    mysql_real_connect(SQLconnectPoolStack.top(), HOST, USER, PASSWORD, TABLE, cti(SQL_PORT), NULL, 0);
                    mysql_set_character_set(SQLconnectPoolStack.top(), "GBK");
                }
            }
            _return = SQLconnectPoolStack.top();
            SQLconnectPoolStack.pop();
            SQLconnectBusySet.insert(_return);
            isbusy = false;
            finished = true;
            return _return;
        }
    }
    return nullptr;
}

void SQLconnectPool::unapplySQLConnect(MYSQL *input)
{
    SQLconnectBusySet.erase(input);
    SQLconnectPoolStack.push(input);
}

//-----------------------control-part------------------------------------------------------

void SQLWrite(SQLconnectPool &pool, std::map<std::string, std::string> rec_c, bool &isbusy)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    // receiveTime,Nodename,data1
    std::string tempSQL = "INSERT INTO sba.nodedata (receiveTime,Nodename,airPressure,lumen,humidity) VALUES (" + rec_c["receiveTime"] + ",\"" + rec_c["Nodename"] + "\",\"" + rec_c["airPressure"] + "\",\"" + rec_c["lumen"] + "\",\"" + rec_c["humidity"] + "\")";
    mysql_query(_SQLconnect, tempSQL.c_str());
    pool.unapplySQLConnect(_SQLconnect);
}

void SQLClear(SQLconnectPool &pool, bool &isbusy)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    std::string tempSQL = "DELETE FROM sba.nodedata";
    mysql_query(_SQLconnect, tempSQL.c_str());
    pool.unapplySQLConnect(_SQLconnect);
}

std::string SQLNewName(SQLconnectPool &pool, bool &isbusy)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    std::string returnval;
    std::string tempSQL = "SELECT count(*) FROM sba.nodestatus";
    mysql_query(_SQLconnect, tempSQL.c_str());
    MYSQL_RES *SQLres = mysql_store_result(_SQLconnect);
    char **row = mysql_fetch_row(SQLres);
    std::string _val = row[0];
    returnval = "ESP" + _val;
    pool.unapplySQLConnect(_SQLconnect);
    return returnval;
}

void SQLBindName(SQLconnectPool &pool, bool &isbusy, long long createtime, std::string name)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    std::string tempSQL = "INSERT INTO sba.nodestatus  (Nodename ,createTime) VALUES (\"" + name + "\"," + std::to_string(createtime) + ")";
    mysql_query(_SQLconnect, tempSQL.c_str());
    pool.unapplySQLConnect(_SQLconnect);
}

void SQLGetData(SQLconnectPool &pool, std::string DATAname, std::string Nodename, std::string &receiveTimeLists, std::string &dataLists, bool &isbusy)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    const std::string query = "SELECT receiveTime," + DATAname + " FROM sba.nodedata WHERE Nodename=\"" + Nodename + "\"";
    mysql_query(_SQLconnect, query.c_str());
    MYSQL_RES *SQLres = mysql_store_result(_SQLconnect);
    int rowCount = 0;
    while (char **row = mysql_fetch_row(SQLres))
    {
        std::string temp1 = row[0], temp2 = row[1];
        rowCount++;
        if (mysql_num_rows(SQLres) != rowCount)
        {
            receiveTimeLists += temp1 + ",";
            dataLists += temp2 + ",";
        }
        else
        {
            receiveTimeLists += temp1;
            dataLists += temp2;
        }
    }
    mysql_free_result(SQLres);
    pool.unapplySQLConnect(_SQLconnect);
}

std::string SQLGetNodeNameList(SQLconnectPool &pool, bool &isbusy)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    const std::string query = "SELECT Nodename FROM sba.nodestatus";
    int rowCount = 0;
    std::string NodeNameList;
    mysql_query(_SQLconnect, query.c_str());
    MYSQL_RES *SQLres = mysql_store_result(_SQLconnect);
    
        while (char **row = mysql_fetch_row(SQLres))
        {
            rowCount++;
            if (mysql_num_rows(SQLres) != rowCount)
            {
                NodeNameList += "\"";
                NodeNameList += row[0];
                NodeNameList += "\",";
            }
            else
            {
                NodeNameList += "\"";
                NodeNameList += row[0];
                NodeNameList += "\"";
            }
        }
    
    mysql_free_result(SQLres);
    pool.unapplySQLConnect(_SQLconnect);
    return NodeNameList;
}

std::string GetstatusRight(SQLconnectPool &pool, bool &isbusy, const char *SQLserverip, const char *SQLserverPort, const char *serverPort)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    //--------------------------------------------------------------------
    std::string status = "";
    std::map<std::string, std::string> Nodestatus, lifestatus, recordstatus, data1AVR, data2AVR, data3AVR, nodestatus;
    MYSQL_RES *SQLres;
    std::string query;
    //--------------------------------------------------------------------
    //************************************
    // right
    //************************************
    query = R"(select * from sba.nodestatus u )";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        nodestatus[row[0]] = row[1];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = "SELECT count(Nodename) FROM sba.nodestatus";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        Nodestatus["NodeNumbers"] = row[0];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    status += "[Node register time]\n";
    for (int it = 0; it != std::stoi(Nodestatus["NodeNumbers"]); it++)
    {
        std::string Nodename = "ESP" + std::to_string(it);
        status += Nodename + ":" + nodestatus[Nodename] + "\n";
    }
    //--------------------------------------------------------------------
    pool.unapplySQLConnect(_SQLconnect);
    return status;
}

std::string GetstatusLeft(SQLconnectPool &pool, bool &isbusy, const char *SQLserverip, const char *SQLserverPort, const char *serverPort)
{
    MYSQL *_SQLconnect = pool.applySQLConnect(isbusy);
    //--------------------------------------------------------------------
    std::string status = "";
    std::map<std::string, std::string> Nodestatus, lifestatus, recordstatus, data1AVR, data2AVR, data3AVR, nodestatus;
    MYSQL_RES *SQLres;
    std::string query;
    //--------------------------------------------------------------------
    //************************************
    // left
    //************************************
    query = "SELECT Nodename ,count(*) FROM sba.nodedata GROUP by Nodename";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        recordstatus[row[0]] = row[1];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = "SELECT count(Nodename) FROM sba.nodestatus";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        Nodestatus["NodeNumbers"] = row[0];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = "select count(distinct Nodename) FROM sba.nodedata";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        Nodestatus["acturalNodeNumber"] = row[0];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = "SELECT Nodename,cast(avg(airPressure) as decimal(38, 2)) FROM sba.nodedata GROUP by Nodename";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        data1AVR[row[0]] = row[1];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = "SELECT Nodename,cast(avg(lumen) as decimal(38, 2)) FROM sba.nodedata GROUP by Nodename";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        data2AVR[row[0]] = row[1];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = "SELECT Nodename,cast(avg(humidity) as decimal(38, 2)) FROM sba.nodedata GROUP by Nodename";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        data3AVR[row[0]] = row[1];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    query = R"(select Nodename,if(REPLACE(unix_timestamp(NOW(3)), '.', '')-max(receiveTime)<1000,"life","dead")  FROM sba.nodedata group by Nodename )";
    mysql_query(_SQLconnect, query.c_str());
    SQLres = mysql_store_result(_SQLconnect);
    while (char **row = mysql_fetch_row(SQLres))
    {
        lifestatus[row[0]] = row[1];
    }
    mysql_free_result(SQLres);
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    status += "[server status]\n";
    status += "SQLserver->";
    status += SQLserverip;
    status += ":";
    status += SQLserverPort;
    status += "\nserver listen to->*";
    status += ":";
    status += serverPort;
    //--------------------------------------------------------------------
    status +=
        "\n\n[Nodestatus]\nRegister node numbers: " + Nodestatus["NodeNumbers"] + "\nActive node numbers: " + Nodestatus["acturalNodeNumber"];
    status += "\n\n[life status]\n";
    for (auto it = lifestatus.begin(); it != lifestatus.end(); it++)
    {
        status += it->first + ":" + it->second + "\n";
    }
    //--------------------------------------------------------------------
    status += "\n[number of records]\n";
    for (auto it = recordstatus.begin(); it != recordstatus.end(); it++)
    {
        status += it->first + ":" + it->second + "\n";
    }
    //--------------------------------------------------------------------
    status += "\n[average data]\n";
    status += "NodeName | airPressure | lumen | humidity\n";
    auto it1 = data1AVR.begin();
    auto it2 = data2AVR.begin();
    auto it3 = data3AVR.begin();
    for (; it1 != data1AVR.end(); it1++)
    {
        status += it1->first + " | " + it1->second + " | " + it2->second + " | " + it3->second + "\n";
        it2++;
        it3++;
    }
    //--------------------------------------------------------------------
    pool.unapplySQLConnect(_SQLconnect);
    return status;
}
