#include "httplib.h"
#include <string>
#include <map>
#include <sys/timeb.h>
#include "html.h"
#include "json.h"
#include "yoursql.h"
#include "mysql.h"
#include <iostream>
#include <fstream>
#include "tools.h"

timeb t;
bool isbusy = false;
json configJson;
httplib::Server server;

int main()
{
    std::string errormessage = initCheck(configJson);
    if (errormessage == "ok")
    {
        const char *serverPort = configJson.record["serverPort"].c_str();
        const char *SQLserverip = configJson.record["SQLserverip"].c_str();
        const char *SQLserverPort = configJson.record["SQLserverPort"].c_str();
        const char *username = configJson.record["username"].c_str();
        const char *password = configJson.record["password"].c_str();
        const char *table = "sba";
        std::cout << "**********************************************************" << std::endl;
        std::cout << "[Server log]Server begin!" << std::endl;
        std::cout << "[Server log]Server listen on *:" << serverPort << std::endl;
        std::cout << "**********************************************************" << std::endl;
        std::cout << "[TIPS]Please open localhost:" << serverPort << " to view the website" << std::endl;
        std::cout << "**********************************************************" << std::endl;
        SQLconnectPool dataPool(SQLserverip, SQLserverPort, username, password, table);

        //---------------------------------------------------------------------------------
        server.Post("/postData", [&](const httplib::Request &req, httplib::Response &res)
                    {
                std::cout<<req.body<<std::endl;
                json reqdata;
                reqdata.json_to_map(req.body);
                reqdata.json_to_map("receiveTime:" + getTimeStampinString(t));
                std::string tempContent;
                tempContent += std::to_string(reqdata.record.size()) + " records had received\n";
                for (auto it = reqdata.record.begin(); it != reqdata.record.end(); it++)
                {
                    tempContent += R"(    [)" + it->first + ':' + it->second + "]" + "\n";
                }
                SQLWrite(dataPool, reqdata.record, isbusy);
                reqdata.record.clear();
                res.set_content(tempContent, "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Post("/postSetup", [&](const httplib::Request &req, httplib::Response &res)
                    {
                writeSetup(req.body);
                std::string tempSetup="received\nSetupfile:"+req.body+"\nThe setup will be enable in next initialization\nplease manually return to main page";
                res.set_content(tempSetup, "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/BindName", [&](const httplib::Request &req, httplib::Response &res)
                   {

                std::multimap<std::string, std::string>::const_iterator it = req.params.begin();
                if(it->first=="key" and it->second=="ThisIsAKeyToBind"){ 
                    SQLBindName(dataPool,isbusy,getTimeStamp(t),SQLNewName(dataPool, isbusy));
                    res.set_content("success", "text/plain");

                }else{
                    res.set_content("keyError", "text/plain");

                } });
        //---------------------------------------------------------------------------------
        server.Get("/GetGraph", [&](const httplib::Request &req, httplib::Response &res)
                   { 

                std::string content,receiveTimeLists, dataLists,DATAname,Nodename;
                if (req.has_param("Nodename") and req.has_param("DATAname")) {
                    Nodename = req.get_param_value("Nodename");
                    DATAname = req.get_param_value("DATAname");
                    SQLGetData(dataPool,DATAname,Nodename,receiveTimeLists, dataLists,isbusy);
                    res.set_content(getGraphContent(receiveTimeLists,dataLists), "text/html"); 
                }else{
                    res.set_content("paramsError", "text/html"); 
                } });
        //---------------------------------------------------------------------------------
        server.set_error_handler([](const httplib::Request &req, httplib::Response &res)
                                 {
                std::cerr << "Server-error:" << req.method << "\t" << req.path << "\tstatus:" << res.status << std::endl;
                res.set_content(R"({"message":"path error"})", "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/resetGraph", [&](const httplib::Request &req, httplib::Response &res)
                   { SQLClear(dataPool,isbusy); 
                res.set_redirect("/", 303); });
        //---------------------------------------------------------------------------------
        server.Get("/NewName", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(SQLNewName(dataPool, isbusy), "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/GetNodeNameList", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(SQLGetNodeNameList(dataPool, isbusy), "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/GetBindKey", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content("ThisIsAKeyToBind", "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/GetstatusLeft", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(GetstatusLeft(dataPool, isbusy, SQLserverip, SQLserverPort, serverPort), "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/GetstatusRight", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(GetstatusRight(dataPool, isbusy, SQLserverip, SQLserverPort, serverPort), "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/index.css", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(getIndexCSS(), "text/css"); });
        //---------------------------------------------------------------------------------
        server.Get("/favicon.ico", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content("404", "text/plain"); });
        //---------------------------------------------------------------------------------
        server.Get("/status.html", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(getStatusHTML(), "text/html"); });
        //---------------------------------------------------------------------------------
        server.Get("/setup.html", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(getSetupHTML(), "text/html"); });
        //---------------------------------------------------------------------------------
        server.Get("/", [&](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(getIndexHTML(SQLGetNodeNameList(dataPool, isbusy)), "text/html"); });
        //---------------------------------------------------------------------------------
        server.listen("0.0.0.0", cti(serverPort));
    }
    else
    {
        std::cout << "[error]errormessage:" << errormessage << std::endl;
        system("pause");
        return 0;
    }
}
