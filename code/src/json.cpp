#include <iostream>
#include <string>
#include <map>
#include "json.h"

void json::json_to_map(std::string data)
{
    std::string skip = R"({}[] '")";
    for (auto it = data.begin(); it != data.end() + 1; it++)
    {
        if (*it == ':' or *it == '=')
        {
            key_done = 1;
        }
        else if (*it == ',' or it == data.end() or *it == '&')
        {
            key_done = 0;
            record[key] = value;
            key = "";
            value = "";
        }
        else if (*it == skip[0] or *it == skip[1] or *it == skip[2] or *it == skip[3] or *it == skip[4] or *it == skip[5] or *it == skip[6] )
        {
            // do nothing
        }
        else if (key_done)
        {
            value += *it;
        }
        else
        {
            key += *it;
        }
    }
}
