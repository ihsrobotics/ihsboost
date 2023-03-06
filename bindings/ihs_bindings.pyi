"""!
@file ihs_bindings.py
@author Eliot Hall
@brief Python bindings for ihs boost
@version 0.1
@date 2023-03-04

@copyright Copyright (c) 2023

@defgroup python_functions_id Python Functions
@{
"""

class CommunicatorBase:
    """
    Base class for all communication
    """

class SocketServer(CommunicatorBase):
    def __init__(self, port: int, max_msg_size: int = ...) -> None:
        """
        Start a socket server on the given port. Messages will have
        max size of `max_msg_size`.
            @param port - what port to host on
            @param max_msg_size - the maximum size for messages
        """

class SocketClient(CommunicatorBase):
    def __init__(self, ip: str, max_msg_size: int = ...) -> None:
        """
        Open a socket client to the given ip and port. Messages will have
        max size of `max_msg_size`.
            @param ip - what ip to connect to
            @param port - what port to connect to
            @param max_msg_size - the maximum size for messages
        """

class SHMCommunicator(CommunicatorBase):
    def __init__(self, id: int, max_msg_size: int = ...) -> None:
        """
        Create a SHMCommunicator to communicate over shared memory
            @param id - the id for the shared memory
            @param max_msg_size - the maximum size for messages
        """

class PosixQCommunicator(CommunicatorBase):
    def __init__(self, name: str, max_msgs: int = ..., max_msg_size: int = ...) -> None:
        """
        Create a PosixQCommunicator
            @param name - the name for this posix queue
            @param max_msgs - the maximum number of messages that the
            posix q will be able to hold before it is considered full
            @param max_msg_size the maximum size for messages
        """

class SysVCommunicator(CommunicatorBase):
    def __init__(self, id: int, max_msg_size: int = ...) -> None:
        """
        Construct a SysVCommunicator
            @param id - the id of the SysVCommunicator
            @param max_msg_size - the maximum size for messages
        """

class Speed:
    def __init__(self, left_speed: int, right_speed: int) -> None:
        """
        Construct a Speed object
            @param left_speed - the speed of the left wheel
            @param right_speed - the speed of the right wheel
        """
    def __eq__(self, __o: object) -> bool:
        """
        Check if the other speed is equal by comparing left and right speeds
            @param __o - the speed to check equality with
        @return true - if they are equal
        @return false - if they aren't equal
        """
    def __ne__(self, __o: object) -> bool:
        """
        Check if the other speed is not equal by comparing left and right speeds
            @param __o the speed to check inequality with
        @return true - if they aren't equal
        @return false - if they are equal
        """
    def __add__(self, __o: object) -> Speed:
        """
        Return a new Speed object representing the sum of two speeds. This is equivalent
        to constructing a new Speed object with speeds `left + __o.left` and `right + __o.right`,
        respectively
            @param __o the other speed
        @return Speed
        """
    def __sub__(self, __o: object) -> Speed:
        """
        Return a new Speed object representing the difference of two speeds. This is equivalent
        to constructing a new Speed object with speeds `left - o.left` and `right - o.right`,
        respectively
            @param o the other speed
        @return Speed
        """
    def __repr__(self) -> str:
        """
        Return a string representation of this object
        """
    ## the left speed
    left: int
    ## the right speed
    right: int

class Timer:
    def __init__(self, time: float) -> None:
        """
        Construct a Timer object
            @param time - how long to set the timer for, in seconds
        """
    def __call__(self) -> bool:
        """
        Returns false until it is time to stop (when it has been `time` seconds)
            @return true - once it has been `time` seconds
            @return false - if it hasn't been `time` seconds yet
        """
    def done(self) -> bool:
        """
        Returns false until it is time to stop (when it has been `time` seconds)
            @return true - once it has been `time` seconds
            @return false - if it hasn't been `time` seconds yet
        """

class Cliff:
    ## value for left cliff
    LCliff = ...
    ## value for left front cliff
    LFCliff = ...
    ## value for right front cliff
    RFCliff = ...
    ## value for right cliff
    RCliff = ...

class LineSide:
    ## value for left side of the line
    LeftSide = ...
    ## value for right side of the line
    RightSide = ...

class CreateExtraController:
    def __init__(self) -> None:
        """
        Create a CreateExtraController object
        """
    def run_main_brush(self, speed: int) -> None:
        """
        Run the main brush at the specified speed. Positive values cause
        it to turn inwards, negative values cause it to turn outwards.
            @param speed How fast to turn the main brush, ranges from -127 to 127
        """
    def run_side_brush(self, speed: int) -> None:
        """
        Run the side brush at the specified speed. Positive values cause
        it to turn CCW, negative values cause it to turn CW
            @param speed How fast to turn the side brush, ranges from -127 to 127
        """
    def run_vacuum(self, speed: int) -> None:
        """
        Run the vacuum at the specified speed. Only positive values are valid.
            @param speed How fast to turn the vacuum, ranges from 0 to 127
        """

def receive_ints(communicator: CommunicatorBase) -> list:
    """
    Receive a list of ints from the communicator.
        @param communicator - the communicator to receive messages through.
    Exceptions:
        Boost.Python.ArgumentError - happens when passed arguments are of incorrect types
        TypeError - happens when c++ is unable to automatically typecast one of the
        arguments provided to the desired type
    """

def receive_doubles(communicator: CommunicatorBase) -> list:
    """
    Receive a list of doubles from the communicator.
        @param communicator - the communicator to receive messages through.
    Exceptions:
        Boost.Python.ArgumentError - happens when passed arguments are of incorrect types
        TypeError - happens when c++ is unable to automatically typecast one of the
        arguments provided to the desired type
    """

def receive_bools(communicator: CommunicatorBase) -> list:
    """
    Receive a list of booleans from the communicator.
        @param communicator - the communicator to receive messages through.
    Exceptions:
        Boost.Python.ArgumentError - happens when passed arguments are of incorrect types
        TypeError - happens when c++ is unable to automatically typecast one of the
        arguments provided to the desired type
    """

def receive_string(communicator: CommunicatorBase) -> str:
    """
    Receive a string from the communicator.
        @param communicator - the communicator to receive messages through.
    Exceptions:
        Boost.Python.ArgumentError - happens when passed arguments are of incorrect types
        TypeError - happens when c++ is unable to automatically typecast one of the
        arguments provided to the desired type
    """

def send_ints(communicator: CommunicatorBase, ints: list) -> bool:
    """
    Send a list of ints through the communicator.
        @param communicator - the communicator to send the message with
        @param ints - the list of ints to send
    Exceptions:
        RuntimeError (BadBufCastException) - happens when trying to retreive
        the wrong type of values from the communicator
    """

def send_doubles(communicator: CommunicatorBase, doubles: list) -> bool:
    """
    Send a list of doubles through the communicator.
        @param communicator - the communicator to send the message with
        @param doubles - the list of doubles to send
    Exceptions:
        RuntimeError (BadBufCastException) - happens when trying to retreive
        the wrong type of values from the communicator
    """

def send_bools(communicator: CommunicatorBase, bools: list) -> bool:
    """
    Send a list of booleans through the communicator.
        @param communicator - the communicator to send the message with
        @param bools - the list of booleans to send
    Exceptions:
        RuntimeError (BadBufCastException) - happens when trying to retreive
        the wrong type of values from the communicator
    """

def send_string(communicator: CommunicatorBase, string: str) -> bool:
    """
    Send a string through the communicator.
        @param communicator - the communicator to send the message with
        @param string - the string to send
    Exceptions:
        RuntimeError (BadBufCastException) - happens when trying to retreive
        the wrong type of values from the communicator
    """

def set_extra_config(config_file: str) -> None:
    """
    Load tunable ihsboost variables from the given config file.
    The extra config file doesn't need to contain all the tunable ihsboost variables
        @param config_file the name of the config file
    """

def set_default_config(config_file: str) -> None:
    """
    Load tunable ihsboost variables from the given config file.
    The default config file should contain all the tunable ihsboost variables.
        @param config_file the name of the config file"
    """

def move_servo_slowly(
    port: int, position: int, speed: int = ..., updates_per_sec: int = ...
) -> None:
    """
    Move a servo slowly to its goal position
        @param port the port where the servo is
        @param position the goal position to move the servo to
        @param speed how fast to move the servo, in ticks per second
        @param updates_per_sec How many updates to do per second
    """

def accelerate_forward_linear(
    from_speed: int,
    to_speed: int,
    accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Accelerate both wheels at the same linear rate
        @param from_speed the starting speed
        @param to_speed the ending speed
        @param accel_per_sec how much to accelerate per second (default 500)
        @param updates_per_sec how many updates to the speed to make per second (default 500)
    """

def accelerate_forward_sin(
    from_speed: int,
    to_speed: int,
    avg_accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Accelerate both wheels at the same sinusoidal rate
        @param from_speed the starting speed
        @param to_speed the ending speed
        @param avg_accel_per_sec how much to accelerate per second (default 500)
        @param updates_per_sec how many updates to the speed to make per second (default 500)
    """

def accelerate_linear(
    from_speed: Speed,
    to_speed: Speed,
    accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Accelerate both wheels at potentially different linear rates
        @param from_speed the starting speed, in the form of {left_speed, right_speed}
        @param to_speed the ending speed, in the form of {left_speed, right_speed}
        @param accel_per_sec how much to accelerate per second (default 500)
        @param updates_per_sec how many updates to the speed to make per second (default 500)
    """

def accelerate_sinusoidal(
    from_speed: Speed,
    to_speed: Speed,
    accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Accelerate both wheels at potentially different sinusoidal rates
        @param from_speed the starting speed, in the form of {left_speed, right_speed}
        @param to_speed the ending speed, in the form of {left_speed, right_speed}
        @param accel_per_sec how much to accelerate per second (default 500)
        @param updates_per_sec how many updates to the speed to make per second (default 500)
    """

def gyro_drive_straight(
    from_speed: int,
    to_speed: int,
    stop_function: function,
    correction_proportion: float = ...,
    accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Drive the create straight using the gyroscope
        @param from_speed speed to start at
        @param to_speed speed to cap at
        @param stop_function a function that returns true when it is time to stop driving forward
        @param correction_proportion how much the correction is by,
        should be between (0, 1)
        @param accel_per_sec how fast the create will accelerate by
        @param updates_per_sec how many updates the function will do per sec
    """

def gyro_turn_degrees(
    from_speed: Speed,
    to_speed: Speed,
    degrees: float,
    accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Turn the create a certain number of degrees using the gyroscope
        @param from_speed the speed to start at
        @param to_speed the speed to end at
        @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
        @param accel_per_sec how fast to accelerate
        @param updates_per_sec how many updates the function will do per sec
    """

def gyro_turn_degrees_v2(
    max_speed: int,
    degrees: float,
    min_speed: int = ...,
    accel_per_sec: float = ...,
    updates_per_sec: float = ...,
) -> None:
    """
    Turns the create, starting at rest and ending at rest, and turning at max
    at `max_speed`
        @param max_speed - the maximum speed to turn at, use a positive value
        @param degrees - how many degrees to turn (+ vals for CW, - vals for CCW)
        @param min_speed - the minimum speed to turn at, use a positive value
        @param accel_per_sec - how fast to accelerate
        @param updates_per_sec - how many updates the function will do per sec
    """

def is_black(val: int) -> bool:
    """
    Returns whether or not a sensor reading is black
        @param val - the sensor value
        @return true - if it is black
        @return false  -if it isn't black
    """

def is_white(val: int) -> bool:
    """
    Returns whether or not a sensor reading is white
        @param val - the sensor value
        @return true - if it is white
        @return false - if it isn't white
    """

def line_follow_basic(
    speed: int,
    cliff_sensor: Cliff,
    line_side: LineSide,
    stop_condition: function,
    stop: bool = ...,
    correction_proportion: float = ...,
    black_val: int = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Line follow at a given speed
        @param speed - the speed to line follow at
        @param cliff_sensor - Which cliff sensor to use \see Cliff
        @param line_side - which side of the line to follow at \see LineSide
        @param stop_condition - a function that returns true when it is time to stop line-following
        @param stop - whether or not to do a full stop after finishing
        @param correction_proportion - how large the correction should be while line following,
        should be between (0, 1)
        @param black_val - value for black
        @param updates_per_sec - how many updates the function will do per sec
    """

def line_follow_accelerate_linear(
    from_speed: int,
    to_speed: int,
    cliff_sensor: Cliff,
    line_side: LineSide,
    stop_condition: function,
    stop: bool = ...,
    correction_proportion: float = ...,
    accel_per_sec: float = ...,
    black_val: int = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Line follow, but accelerate linearly
        @param from_speed - speed to start at
        @param to_speed - speed to finish line-following at
        @param cliff_sensor - which cliff sensor to use \see Cliff
        @param line_side - which side of the line to follow at \see LineSide
        @param stop_condition - a function that returns true when it is time to stop line-following
        @param stop - whether or not to do a full stop after finishing
        @param correction_proportion - how large the correction should be while line following,
        should be between (0, 1)
        @param accel_per_sec - how fast to accelerate
        @param black_val - value for black
        @param updates_per_sec - how many updates the function will do per sec
    """

def line_follow_accelerate_sinusoidal(
    from_speed: int,
    to_speed: int,
    cliff_sensor: Cliff,
    line_side: LineSide,
    stop_condition: function,
    stop: bool = ...,
    correction_proportion: float = ...,
    accel_per_sec: float = ...,
    black_val: int = ...,
    updates_per_Sec: int = ...,
) -> None:
    """
    Line follow, but accelerate sinusoidally
        @param from_speed - speed to start at
        @param to_speed - speed to finish line-following at
        @param cliff_sensor - which cliff sensor to use \see Cliff
        @param line_side - which side of the line to follow at \see LineSide
        @param stop_condition - a function that returns true when it is time to stop line-following
        @param stop - whether or not to do a full stop after finishing
        @param correction_proportion - how large the correction should be while line following,
        should be between (0, 1)
        @param accel_per_sec - how fast to accelerate
        @param black_val - value for black
        @param updates_per_sec - how many updates the function will do per sec
    """

def align_with_black(
    speed: int,
    correction_speed: int,
    cliff_sensor_l: Cliff,
    cliff_sensor_r: Cliff,
    stop: bool = ...,
    updates_per_second: int = ...,
) -> None:
    """
    Align with black, meaning keep moving until both `cliff_sensor_l` and `cliff_sensor_r` are on black.
        @param speed - the speed to go when not on black
        @param correction_speed - the speed to go at once on black
        @param cliff_sensor_l - the left cliff sensor to use
        @param cliff_sensor_r - the right cliff sensor to use
        @param stop - whether or not to do a full stop after aligning
        @param updates_per_second - how many updates to do per second.
    """

def align_with_white(
    speed: int,
    correction_speed: int,
    cliff_sensor_l: Cliff,
    cliff_sensor_r: Cliff,
    stop: bool = ...,
    updates_per_second: int = ...,
) -> None:
    """
    Align with white, meaning keep moving until both `cliff_sensor_l` and `cliff_sensor_r` are on white.
        @param speed - the speed to go when not on white
        @param correction_speed - the speed to go at once on white
        @param cliff_sensor_l - the left cliff sensor to use
        @param cliff_sensor_r - the right cliff sensor to use
        @param stop - whether or not to do a full stop after aligning
        @param updates_per_second - how many updates to do per second.
    """

def rotate(
    leftWheelSpeed: float,
    rightWheelSpeed: float,
    angle: float,
    left_wheel_units: float = ...,
    right_wheel_units: float = ...,
) -> None:
    """
    Rotate using speed-based turning on the roomba
        @param leftWheelSpeed - double - it is speed of left wheel in CM per second. Use values from [-50, 50]
        @param rightWheelSpeed - double - it is speed of right wheel in CM per second. Use values from [-50, 50]
        @param angle - double - angle to turn to in degrees
        @param left_wheel_units - how far the left wheel moves, in cm/sec
        @param right_wheel_units - how far the right wheel moves, in cm/sec
    """

def encoder_drive_straight_cm(
    speed: int,
    cm: float,
    stop: bool = ...,
    min_speed: int = ...,
    correction_proportion: float = ...,
    accel_per_sec: float = ...,
    updates_per_sec: float = ...,
) -> None:
    """
    Drive straight using create encoders. Uses the following formula:
    `N counts * (mm in 1 wheel revolution / counts in 1 wheel revolution) = mm`

        @param speed - A positive value representing the absolute value of the max speed to travel at
        @param cm - how many centimeters to travel
        @param stop - whether or not to do a full stop after finishing
        @param min_speed - the minimum speed to travel at
        @param correction_proportion - how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
        @param accel_per_sec - how fast to accelerate per second
        @param updates_per_sec - how many updates to do per second
    """

def encoder_drive_straight_fn(
    speed: int,
    condition: function,
    stop: bool = ...,
    correction_proportion: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Drive straight at `speed` until it is time to stop
        @param speed - the speed to drive at, can be positive or negative
        @param condition - a function that returns true when it is time to stop
        @param stop - whether or not to do a full stop after aligning
        @param correction_proportion - how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
        @param updates_per_sec - how many updates to do per second
    """

def encoder_drive_straight_pid(
    speed: int,
    cm: float,
    proportional_coefficient: float,
    integral_coefficient: float,
    derivative_coefficient: float,
    stop: bool = ...,
    min_speed: int = ...,
    accel_per_sec: float = ...,
    updates_per_second: int = ...,
) -> None:
    """
    Drive the create straight using create encoders and PID control (Proportional/Integral/Derivative).
    Requires tuning of proportional, integral, and derivative coefficients for good results. \see PIDController

    @param speed - the max speed to move at
    @param cm - how many centimeters to move
    @param proportional_coefficient - the coefficient for proportionality to the error
    @param integral_coefficient - the coefficient for the integral of the error
    @param derivative_coefficient - the coefficient for the derivative of the error
    @param stop - whether or not to do a full stop after aligning
    @param min_speed - the minimum speed to drive at
    @param accel_per_sec - how fast to accelerate per second
    @param updates_per_second - how many updates to do per second
    """

def encoder_turn_degrees(
    turn_speed: Speed, degrees: float, updates_per_sec: int = ...
) -> None:
    """
    Turns a certain number of degrees using create encoders
    This overload doesn't accelerate / decelerate; instead,
    it merely turns at the given speed until reaching the goal degrees
        @param turn_speed - The speed to turn at. \see Speed
        @param degrees - The number of degrees to turn, positive values for CW
        @param updates_per_sec - How many updates to do per second
    """

def encoder_turn_degrees_v2(
    max_speed: int,
    degrees: float,
    min_speed: int = ...,
    accel_per_sec: float = ...,
    updates_per_sec: int = ...,
) -> None:
    """
    Turns a certain number of degrees using create encoders.
    angle in radians = (left wheel distance - right wheel distance) / wheel base distance.
        @param max_speed - The maximum speed to turn at. Should be greater than 0.
        @param degrees - The number of degrees to turn, positive values for CW
        @param min_speed - The minimum speed to turn at when decelerating
        @param accel_per_sec - How fast to accelerate per second
        @param updates_per_sec - How many updates to do per second
    """

## @}
