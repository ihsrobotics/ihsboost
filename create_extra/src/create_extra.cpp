#include "create_extra.hpp"
#include <kipr/wombat.h>

CreateExtraController::CreateExtraController() : write(0){};

void CreateExtraController::set_side_brush_direction_CCW()
{
    write &= ~(0b00001000);
}
void CreateExtraController::set_side_brush_direction_CW()
{
    write |= 0b00001000;
}

void CreateExtraController::run_side_brush()
{
    write |= 0b00000001;
}
void CreateExtraController::turn_off_side_brush()
{
    write &= ~(0b00000001);
}

void CreateExtraController::run_vacuum()
{
    write |= 0b00000010;
}
void CreateExtraController::turn_off_vacuum()
{
    write &= ~(0b00000010);
}

void CreateExtraController::run_main_brush()
{
    write |= 0b00000100;
}
void CreateExtraController::turn_off_main_brush()
{
    write &= ~(0b00000100);
}

void CreateExtraController::set_main_brush_direction_outward()
{
    write |= 0b00010000;
}
void CreateExtraController::set_main_brush_direction_inward()
{
    write &= ~(0b00010000);
}

void CreateExtraController::transfer()
{
    create_write_byte(138);
    create_write_byte(write);
}