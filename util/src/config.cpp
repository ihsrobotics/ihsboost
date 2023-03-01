#include "config.hpp"
#include <memory>

std::shared_ptr<Config> ihsboost_config(nullptr);

Config &get_config(std::string config_file)
{
    if (ihsboost_config == nullptr)
    {
        ihsboost_config = std::make_shared<Config>(config_file);
    }
    return *ihsboost_config.get();
}