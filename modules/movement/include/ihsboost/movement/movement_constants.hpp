/**
 * @file movement_constants.hpp
 * @author Eliot Hall
 * @brief Movement constants
 * @version 0.1
 * @date 2023-04-07
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef IHSBOOST_MOVEMENT_CONSTANTS_HPP
#define IHSBOOST_MOVEMENT_CONSTANTS_HPP

#include <math.h>

#define deg2rad_mult 0.017453292519943296 ///< convert degrees to radians by multiplying by this
#define rad2deg_mult 57.29577951308232    ///< convert radians to degrees by multiplying by this
#define DIST_BETWEEN_WHEEL 23.5           ///< distance between the roomba wheels, in cm

#define ENC_2_MM (M_PI * 72.0 / 508.8)   ///< multiply by this to convert enc to mm
#define MM_2_ENC (508.8 / (72.0 * M_PI)) ///< multiply by this to convert mm to enc

#endif
/**@}*/