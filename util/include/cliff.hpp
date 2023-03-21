/**
 * @file cliff.hpp
 * @author Eliot Hall
 * @brief Cliff and Line definitions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef IHSBOOST_CLIFF_HPP
#define IHSBOOST_CLIFF_HPP

/**
 * @brief enum representing the cliff sensors on the create
 *
 */
enum Cliff
{
    LCliff = 0,
    LFCliff,
    RFCliff,
    RCliff
};

/**
 * @brief enum representing which side of the line to follow on
 *
 */
enum LineSide
{
    LeftSide = 0,
    RightSide
};

#endif
/**@}*/