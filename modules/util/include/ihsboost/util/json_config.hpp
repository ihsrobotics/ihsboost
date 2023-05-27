/**
 * @file json_config.hpp
 * @author Snow (Haoyun) Luo, Eliot Hall
 * @brief Class for loading json configs
 * @version 0.1
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef IHSBOOST_JSON_CONFIG_HPP
#define IHSBOOST_JSON_CONFIG_HPP
#include <jsoncpp/json/json.h>
#include <string>

/**
 * @brief Class for loading values from json configs
 *
 */
class Config {
  public:
    /**
     * @brief Construct a new Config object from the given json file
     *
     * @param json_file_name the name of the json file
     */
    Config(std::string json_file_name = "bot-config.json");

    /**
     * @brief Destroy the Config object
     *
     */
    virtual ~Config();

    /**
     * @brief Get the value of `key` as an integer
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to an int
     *
     * @param key the name of the key from the json file
     * @return int the value stored at the given key, or 0 if the key doesn't
     * exist
     */
    virtual int getInt(std::string key);

    /**
     * @brief Get the value of `key` as a double
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to a double
     *
     * @param key the name of the key from the json file
     * @return double the value stored at the given key, or 0 if the key doesn't
     * exist
     */
    virtual double getDouble(std::string key);

    /**
     * @brief Get the value of `key` as a bool
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to a bool
     *
     * @param key the name of the key from the json file
     * @return bool the value stored at the given key, or false if the key
     * doesn't exist
     */
    virtual bool getBool(std::string key);

    /**
     * @brief Get the value of `key` as a bool
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to a string
     *
     * @param key the name of the key from the json file
     * @return std::string the value stored at the given key, or an empty string
     * if the key doesn't exist
     */
    virtual std::string getString(std::string key);

    /**
     * @brief Set a config value to an int
     *
     * @param key the name of the key
     * @param val the value to set it to
     */
    virtual void setInt(std::string key, int val);

    /**
     * @brief Set a config value to a double
     *
     * @param key the name of the key
     * @param val the value to set it to
     */
    virtual void setDouble(std::string key, double val);

    /**
     * @brief Set a config value to a bool
     *
     * @param key the name of the key
     * @param val the value to set it to
     */
    virtual void setBool(std::string key, bool val);

    /**
     * @brief Set a config value to a string
     *
     * @param key the name of the key
     * @param val the value to set it to
     */
    virtual void setString(std::string key, std::string val);

    /**
     * @brief Save the config to a json file
     *
     * @param file the name of the file to save to. If the file doesn't
     * exist, a new file is created.
     */
    virtual void save(std::string file = "bot-config.json");

  protected:
    /**
     * @brief Load values from the given json file
     * @details deletes all previously stored values
     *
     * @param json_file_name the json file to load configs from
     */
    void loadConfig(std::string json_file_name);

  private:
    Json::Value configs; ///< stores the key/value pairs for later retreival
};

#endif
/**@}*/