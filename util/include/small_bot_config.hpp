/**
 * @file small_bot_config.hpp
 * @author Eliot Hall
 * @brief Movement and gyro configs for small bot
 * @version 0.1
 * @date 2023-02-17
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef CONFIG
#define CONFIG

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
#ifndef MEAN_GYRO_VAL
#define MEAN_GYRO_VAL .532 ///< default mean gyro val; used when tuning gyro filter
#endif
#ifndef MIN_GYRO_VAL
#define MIN_GYRO_VAL -4 ///< default min gyro val; used when tuning gyro filter
#endif
#ifndef MAX_GYRO_VAL
#define MAX_GYRO_VAL 5 ///< default max gyro val; used when tuning gyro filter
#endif
#ifndef RAW_TO_360_DEGREES
#define RAW_TO_360_DEGREES 7.5122 ///< default conversion from integrated gyro values to degrees
#endif
#ifndef GYRO_FUNCTION
#define GYRO_FUNCTION gyro_y ///< the function to use for gyro movement, must return a signed short
#endif
#ifndef GYRO_CW_MULTIPLIER
#define GYRO_CW_MULTIPLIER -1 ///< multiply by this so that + values are CW, - values are CCW
#endif

#endif
/**@}*/