#include "config.hpp"
#include <memory>

using namespace std;

/**
 * @brief Class that deals with manages extra configs vs default configs
 *
 */
class PartialConfig : public Config
{
public:
    /**
     * @brief Construct a new Partial Config object from the default config file
     *
     */
    PartialConfig() : Config(CONFIG_FILE), extra_config(nullptr){};

    virtual ~PartialConfig()
    {
        if (extra_config != nullptr)
        {
            delete extra_config;
        }
    }

    /**
     * @brief load extra configs from the config file
     *
     * @param config_file
     */
    void set_extra_config(string config_file)
    {
        if (extra_config != nullptr)
        {
            delete extra_config;
        }
        extra_config = new Config(config_file);
    }

    /**
     * @brief Set the defualt config file to the given file
     *
     * @param config_file
     */
    void set_defualt_config(string config_file)
    {
        loadConfig(config_file);
    }

    /**
     * @brief Return if the extra config doesn't have a value for the given key
     *
     * @param key the key to check if it has a value
     * @return true if the key doesn't have a value
     * @return false if the key does have a value
     */
    bool isEmptyExtraValue(string key)
    {
        if (extra_config == nullptr)
        {
            return true;
        }
        string extra_value = extra_config->getString(key);
        return extra_value.size() == 0;
    }

    /**
     * @brief get the value of `key` as a bool
     *
     * @param key the name of the key from the json file
     * @return bool - the value stored in the extra config if there is one,
     *  or the value stored in the default config,
     *  or false if the key doesn't exist
     */
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

    /**
     * @brief get the value of `key` as an int
     *
     * @param key the name of the key from the json file
     * @return int - the value stored in the extra config if there is one,
     *  or the value stored in the default config,
     *  or 0 if the key doesn't exist
     */
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

    /**
     * @brief get the value of `key` as a double
     *
     * @param key the name of the key from the json file
     * @return double - the value stored in the extra config if there is one,
     *  or the value stored in the default config,
     *  or 0 if the key doesn't exist
     */
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

    /**
     * @brief get the value of `key` as a string
     *
     * @param key the name of the key from the json file
     * @return bool - the value stored in the extra config if there is one,
     *  or the value stored in the default config,
     *  or an empty string if the key doesn't exist
     */
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
    Config *extra_config; ///< the extra config
};

std::shared_ptr<PartialConfig> ihsboost_config(nullptr);

Config &get_config(std::string config_file)
{
    if (ihsboost_config.get() == nullptr)
    {
        ihsboost_config = make_shared<PartialConfig>();
    }
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