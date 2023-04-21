#include "create_extra.hpp"
#include <kipr/wombat.h>

CreateExtraController::CreateExtraController() : main_brush_speed(0), side_brush_speed(0), vacuum_speed(0){};

void CreateExtraController::run_main_brush(int8_t speed)
{
    main_brush_speed = speed;
    transfer();
}

void CreateExtraController::run_side_brush(int8_t speed)
{
    side_brush_speed = speed;
    transfer();
}

void CreateExtraController::run_vacuum(int8_t speed)
{
    vacuum_speed = speed;
    transfer();
}

void CreateExtraController::transfer()
{
    create_write_byte(static_cast<char>(144));
    create_write_byte(static_cast<char>(main_brush_speed));
    create_write_byte(static_cast<char>(side_brush_speed));
    create_write_byte(static_cast<char>(vacuum_speed));
}