#include "json_config.hpp"
#include <fstream>
using namespace std;

// ========== Constructors and deconstructors ==========
Config::Config(string json_file_name) : configs()
{
    ifstream file(json_file_name);
    file >> configs;
    file.close();
}
void Config::loadConfig(string json_file_name)
{
    *this = Config(json_file_name);
}
Config::~Config(){};

// ========== Getters ==========
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

// ========== Setters ==========
void Config::setInt(string key, int val)
{
    configs[key] = val;
}
void Config::setDouble(string key, double val)
{
    configs[key] = val;
}
void Config::setBool(string key, bool val)
{
    configs[key] = val;
}
void Config::setString(string key, string val)
{
    configs[key] = val;
}
void Config::save(string file)
{
    Json::StreamWriterBuilder writer;
    ofstream out(file);
    writer.newStreamWriter()->write(configs, &out);
}