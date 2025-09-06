#include "tools.h"
#include "json.h"
#include "mysql.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

int cti(const char *input)
{
    std::string _input = input;
    int __input = std::stoi(_input);
    return __input;
}

std::string initCheck(json &configdata)
{
    std::fstream config;
    config.open("config.txt", std::ios::in);
    if (!config.is_open())
    {
        config.open("config.txt", std::ios::out);
        config.close();
        config.open("config.txt", std::ios::in);
    }
    std::string tempconfig, finalconfig;
    while (getline(config, tempconfig))
    {
        finalconfig += tempconfig;
    }
    configdata.json_to_map(finalconfig);
    config.close();
    if (configdata.record.size() == 1)
    {
        return "config.txt is empty";
    }
    std::map<std::string, std::string>::const_iterator it = configdata.record.begin();
    for (; it != configdata.record.end(); it++)
    {
        if (it->second == "")
        {
            return "config.txt not full fill";
        }
    }
    try
    {
        std::stoi(configdata.record["serverPort"]);
    }
    catch (std::invalid_argument)
    {
        return "serverPort should be a number";
    }
    catch (std::out_of_range)
    {
        return "serverPort out of range(0-2147483647)";
    }
    try
    {
        std::stoi(configdata.record["SQLserverPort"]);
    }
    catch (std::invalid_argument)
    {
        return "SQLserverPort should be a number";
    }
    catch (std::out_of_range)
    {
        return "SQLserverPort out of range(0-2147483647)";
    }
    MYSQL *_connectTest = mysql_init(NULL);
    if (!mysql_real_connect(_connectTest, configdata.record["SQLserverip"].c_str(), configdata.record["username"].c_str(), configdata.record["password"].c_str(), configdata.record["table"].c_str(), std::stoi(configdata.record["SQLserverPort"]), NULL, 0))
    {
        return "database connect error";
    }
    mysql_close(_connectTest);
    if (std::stoi(configdata.record["SQLserverPort"]) < 0)
    {
        return "SQLserverPort out of range(0-2147483647)";
    }
    if (std::stoi(configdata.record["serverPort"]) < 0)
    {
        return "serverPort out of range(0-2147483647)";
    }
    return "ok";
}

bool writeSetup(std::string SetupDATA)
{
    std::fstream config;
    config.open("config.txt", std::ios::out);
    config << SetupDATA;
    return 0;
}

long long getTimeStamp(timeb &t)
{
    ftime(&t);
    return t.time * 1000 + t.millitm;
}

std::string getTimeStampinString(timeb &t)
{
    ftime(&t);
    return std::to_string(t.time * 1000 + t.millitm);
}
