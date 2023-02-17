/**
 * @file roomba_config.hpp
 * @author Eliot Hall
 * @brief Movement and gyro configs for roomba
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
#ifndef MOVEMENT_FUNCTION
#define MOVEMENT_FUNCTION create_drive_direct ///< movement function for create's
#endif

// gyro configs
#ifndef MEAN_GYRO_VAL
#define MEAN_GYRO_VAL -0.6737848705181358 ///< default mean gyro val for the roomba; used when tuning gyro filter
#endif
#ifndef MIN_GYRO_VAL
#define MIN_GYRO_VAL -5 ///< default min gyro val for the roomba; used when tuning gyro filter
#endif
#ifndef MAX_GYRO_VAL
#define MAX_GYRO_VAL 3 ///< default max gyro val for the roomba; used when tuning gyro filter
#endif
#ifndef RAW_TO_360_DEGREES
#define RAW_TO_360_DEGREES 7.5122 ///< default conversion from integrated gyro values to degrees on the roomba
#endif
#ifndef GYRO_FUNCTION
#define GYRO_FUNCTION gyro_x ///< the function to use for gyro movement, must return a signed short
#endif
#ifndef GYRO_CW_MULTIPLIER
#define GYRO_CW_MULTIPLIER -1 ///< multiply by this so that + values are CW, - values are CCW
#endif

#endif
/**@}*/