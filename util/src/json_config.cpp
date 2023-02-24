#include "json_config.hpp"
#include <fstream>
using namespace std;

Config::Config(string json_file_name) : configs()
{
    ifstream file(json_file_name);
    file >> configs;
    file.close();
}

Config::~Config(){};

int Config::getInt(string key)
{
    return configs[key].asInt();
}

double Config::getDouble(string key)
{
    return configs[key].asDouble();
}

bool Config::getBool(string key)
{
    return configs[key].asBool();
}

string Config::getString(string key)
{
    return configs[key].asString();
}