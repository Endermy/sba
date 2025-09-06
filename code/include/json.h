#pragma once
#include <map>
class json
{
private:
    std::string key, value;
    bool key_done = 0;
public:
    std::map<std::string, std::string> record;
    void json_to_map(std::string data);
};
