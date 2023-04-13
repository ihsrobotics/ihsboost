import ctypes
import ihs_bindings

# connect to create
k = ctypes.CDLL("libkipr.so")
k.create_connect_once()

# test encoders by driving a lot back and forth
for i in range(4):
    ihs_bindings.encoder_drive_straight_cm(300, 50, updates_per_sec=1000, accel_per_sec=1000)
    if i%2 ==0:
        ihs_bindings.encoder_turn_degrees_v2(300, 180, updates_per_sec=1000, accel_per_sec=1000)
    else:
        ihs_bindings.encoder_turn_degrees_v2(300, -180, updates_per_sec=1000, accel_per_sec=1000)

# test other encoder driving by going until lbump is pressed
ihs_bindings.encoder_drive_straight_fn(200, k.get_create_lbump)

# cleanup
k.create_disconnect()