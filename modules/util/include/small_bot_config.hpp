/**
 * @file small_bot_config.hpp
 * @author Eliot Hall
 * @brief Non-jsonable movement and gyro configs for small bot
 * @version 0.1
 * @date 2023-02-17
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef IHSBOOST_CONFIG
#define IHSBOOST_CONFIG

// movement configs
#ifndef LEFT_MOTOR
#define LEFT_MOTOR 0 ///< default left motor port
#endif
#ifndef RIGHT_MOTOR
#define RIGHT_MOTOR 1 ///< default right motor port
#endif
#ifndef MOVEMENT_FUNCTION
#define MOVEMENT_FUNCTION(lspeed, rspeed) \
    {                                     \
        mav(LEFT_MOTOR, lspeed);          \
        mav(RIGHT_MOTOR, rspeed);         \
    } ///< movement function for small bots
#endif

// gyro configs
#ifndef GYRO_FUNCTION
#define GYRO_FUNCTION gyro_y ///< the function to use for gyro movement, must return a signed short
#endif

#endif
/**@}*/