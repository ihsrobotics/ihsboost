import ihs_bindings

t = None


def timer_function() -> bool:
    global t
    if t is None:
        t = ihs_bindings.Timer(2)
    return t.done()


# keyword arguments in python where you can specify the values of each
# argument by doing `argument=value`
ihs_bindings.gyro_drive_straight(0, 100, timer_function, updates_per_sec=100)

# line follow
ihs_bindings.line_follow_basic(
    100,
    ihs_bindings.Cliff.LCliff,  # enums work very similar to in c++, but instead of Clif::, use Cliff.
    ihs_bindings.LineSide.LeftSide,
    timer_function,  # pass functions just like you would to c++
    stop=False,
)

ihs_bindings.accelerate_forward_linear(100, 0, updates_per_sec=200)
