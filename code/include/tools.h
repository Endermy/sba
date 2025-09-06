#pragma once  
#include <iostream>
#include "json.h"
#include <string>

int cti(const char *input);
std::string initCheck(json &configdata);
long long getTimeStamp(timeb &t);
std::string getTimeStampinString(timeb &t);
bool writeSetup(std::string SetupDATA);
