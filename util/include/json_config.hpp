/**
 * @file json_config.hpp
 * @author Snow (Haoyun) Luo, Eliot Hall
 * @brief Class for loading json configs
 * @version 0.1
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef JSON_CONFIG_HPP
#define JSON_CONFIG_HPP
#include <string>
#include <jsoncpp/json/json.h>

/**
 * @brief Class for loading values from json configs
 *
 */
class Config
{
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
    ~Config();

    /**
     * @brief Get the value of `key` as an integer
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to an int
     *
     * @param key the name of the key from the json file
     * @return int the value stored at the given key, or 0 if the key doesn't exist
     */
    int getInt(std::string key);

    /**
     * @brief Get the value of `key` as a double
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to a double
     *
     * @param key the name of the key from the json file
     * @return double the value stored at the given key, or 0 if the key doesn't exist
     */
    double getDouble(std::string key);

    /**
     * @brief Get the value of `key` as a bool
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to a bool
     *
     * @param key the name of the key from the json file
     * @return bool the value stored at the given key, or false if the key doesn't exist
     */
    bool getBool(std::string key);

    /**
     * @brief Get the value of `key` as a bool
     * @exception Json::LogicError if the value of `key`
     * can't be automatically type-cast to a string
     *
     * @param key the name of the key from the json file
     * @return std::string the value stored at the given key, or an empty string if the key doesn't exist
     */
    std::string getString(std::string key);

private:
    Json::Value configs; ///< stores the key/value pairs for later retreival
};

#endif