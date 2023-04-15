/**
 * @file roomba_config.hpp
 * @author Eliot Hall
 * @brief Non-jsonable movement and gyro configs for roomba
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
#ifndef MOVEMENT_FUNCTION
#define MOVEMENT_FUNCTION create_drive_direct ///< movement function for create's
#endif

// gyro configs
#ifndef GYRO_FUNCTION
#define GYRO_FUNCTION gyro_x ///< the function to use for gyro movement, must return a signed short
#endif

// accelerometer configs
#ifndef ACCEL_FUNCTION
#define ACCEL_FUNCTION accel_z ///< the function to use for accelerometer position tracking
#endif

#endif
/**@}*/