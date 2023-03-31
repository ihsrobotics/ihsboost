/**
 * @file json_loader.hpp
 * @author Eliot Hall
 * @brief File that demonstrates example config loader
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef JSON_LOADER_HPP
#define JSON_LOADER_HPP

#include <ihsboost/json_config.hpp>
#include <iostream>
using namespace std;

// load configs; by not providing arguments, it automatically
// loads bot-config.json
Config bot_config;

const double arm_position = bot_config.getDouble("arm position");
const string owner = bot_config.getString("owner");

// load configs with other names
Config cool_config("other-config.json");

const int a = cool_config.getInt("a");
const int b = cool_config.getInt("b");
const int c = cool_config.getInt("c");
const bool is_good = cool_config.getBool("is_good");
const bool is_bad = cool_config.getBool("is_bad");

#endif