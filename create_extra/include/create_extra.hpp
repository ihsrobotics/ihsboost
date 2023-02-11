#ifndef CREATE_EXTRA_HPP
#define CREATE_EXTRA_HPP
#include <cstdint>

/**
 * @brief This class controls extra functions of the create.
 * @details This requires being in full mode
 *
 */
class CreateExtraController
{
public:
    CreateExtraController();

    /**
     * @brief Run the main brush at the specified speed
     * @details positive values cause it to turn inwards,
     * negative values cause it to turn outwards
     *
     * @param speed How fast to turn the main brush, ranges from -127 to 127
     */
    void run_main_brush(int8_t speed);

    /**
     * @brief Run the side brush at the specified speed
     * @details positive values cause it to turn CCW,
     * negative values cause it to turn CW
     *
     * @param speed How fast to turn the side brush, ranges from -127 to 127
     */
    void run_side_brush(int8_t speed);

    /**
     * @brief Run the vacuum at the specified speed
     * @details only positive values are valid.
     *
     * @param speed How fast to turn the vacuum, ranges from 0 to 127
     */
    void run_vacuum(int8_t speed);

private:
    /**
     * @brief Write changes to the create. This must be called
     * in order for changes to take effect.
     *
     */
    void transfer();

    uint8_t write;

    int8_t main_brush_speed;
    int8_t side_brush_speed;
    int8_t vacuum_speed;
};

#endif