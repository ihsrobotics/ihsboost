#ifndef SERVOS_HPP
#define SERVOS_HPP

#define SERVO_MOVEMENT_SPEED 200
#define SERVO_UPDATES_PER_SEC 100

/**
 * @brief Move a servo slowly to its goal position
 *
 * @param port the port where the servo is
 * @param position the goal position to move the servo to
 * @param speed how fast to move the servo, in ticks per second. Defaults to 200
 * @param updates_per_sec How many updates to do per second, defaults to 100
 */
void move_servo_slowly(int port, int position, int speed = SERVO_MOVEMENT_SPEED, int updates_per_sec = SERVO_UPDATES_PER_SEC);

#endif