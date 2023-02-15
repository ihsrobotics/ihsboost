/**
 * @file config.hpp
 * @author Eliot Hall
 * @brief Default configurations for ihsboost
 * @version 0.1
 * @date 2023-02-15
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef MOVEMENT_FUNCTION
#define MOVEMENT_FUNCTION create_drive_direct ///< default movement function, must be a callable that takes two arguments: left_speed, right_speed
#endif

#endif
/**@}*/