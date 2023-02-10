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
     * @brief Set the side brush direction to CCW
     * @details doesn't actually start the side brush
     *
     */
    void set_side_brush_direction_CCW();

    /**
     * @brief Set the side brush direction to CW
     * @details doesn't actually start the side brush
     *
     */
    void set_side_brush_direction_CW();

    /**
     * @brief Run the side brush. If no direction was set,
     * default direction is CCW
     *
     */
    void run_side_brush();

    /**
     * @brief Turn off the side brush.
     *
     */
    void turn_off_side_brush();

    /**
     * @brief Run the vacuum
     *
     */
    void run_vacuum();

    /**
     * @brief Turn off the vacuum
     *
     */
    void turn_off_vacuum();

    /**
     * @brief Turn on the main brush. If no direction was set,
     * then this defaults to inward
     *
     */
    void run_main_brush();

    /**
     * @brief Turn off the main brush.
     *
     */
    void turn_off_main_brush();

    /**
     * @brief Set the main brush direction to inward
     *
     */
    void set_main_brush_direction_inward();

    /**
     * @brief Set the main brush direction to outward
     *
     */
    void set_main_brush_direction_outward();

    /**
     * @brief Write changes to the create. This must be called
     * in order for changes to take effect.
     *
     */
    void transfer();

private:
    uint8_t write;
};

#endif