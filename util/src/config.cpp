#include "config.hpp"
#include <memory>

using namespace std;

class PartialConfig : public Config
{
public:
    PartialConfig() : Config(CONFIG_FILE), extra_config(nullptr){};

    void set_extra_config(string config_file)
    {
        if (extra_config != nullptr)
        {
            delete extra_config;
        }
        extra_config = new Config(config_file);
    }

    void set_defualt_config(string config_file)
    {
        loadConfig(config_file);
    }

    bool isEmptyExtraValue(string key)
    {
        if (extra_config == nullptr)
        {
            return true;
        }
        string extra_value = extra_config->getString(key);
        return extra_value.size() == 0;
    }

    virtual bool getBool(string key) override
    {
        if (isEmptyExtraValue(key))
        {
            return Config::getBool(key);
        }
        else
        {
            return extra_config->getBool(key);
        }
    }

    virtual int getInt(string key) override
    {
        if (isEmptyExtraValue(key))
        {
            return Config::getInt(key);
        }
        else
        {
            return extra_config->getInt(key);
        }
    }
    virtual double getDouble(string key) override
    {
        if (isEmptyExtraValue(key))
        {
            return Config::getDouble(key);
        }
        else
        {
            return extra_config->getDouble(key);
        }
    }
    virtual string getString(string key) override
    {
        if (isEmptyExtraValue(key))
        {
            return Config::getString(key);
        }
        else
        {
            return extra_config->getString(key);
        }
    }

private:
    Config *extra_config;
};

std::shared_ptr<PartialConfig> ihsboost_config(make_shared<PartialConfig>());

Config &get_config(std::string config_file)
{
    return *ihsboost_config.get();
}

void set_extra_config(string config_file)
{
    ihsboost_config.get()->set_extra_config(config_file);
}

void set_default_config(string config_file)
{
    ihsboost_config.get()->set_defualt_config(config_file);
}