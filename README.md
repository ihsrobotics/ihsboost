# IHS Boost
The ihs library of robot functions, from steady acceleration to servo movement
## Installing
### Wombat Build
To build on the wombat, run the following commands
from terminal inside the ihsboost directory
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/
make -j4
sudo make install
```
Note: you will need to use either the `main` branch or the 
`older-wombat` branch depending on whether you are using a
new wombat or an old (original OS) wombat, 
### Cross Compile Build
Currently, Cross compile build is unsupported.
## Compiling programs with it
To compile a program with the ihsboost library, then
assuming that you have already installed ihsboost,
all you have to do is the following command to compile
to form the executable `./a.out`:
```
g++ (file) -lihsboost -std=c++11
```
Note: `(file)` should be replaced by the name of the file that
should be compiled.
## Network Config
### Steps
If you are trying to connect from older wombat to older wombat, then
the following extra steps are required from one of them in order
to connect to the wifi of another wombat:
```
sudo systemctl stop hostapd
sudo systemctl start wpa_supplicant

wpa_password (wifi_name) (wifi_password) | sudo tee /etc/wpa_supplicant.conf
```
Where `(wifi_name)` is the name of the wifi that you want to connect to
and `(wifi_password)` is the password for that wifi network.

Then, in a terminal that you will keep open, run
```
sudo wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &
sudo ifconfig wlan0 (new_ip_address)
```
Where `(new_ip_address)` is the ip address that you want this wombat
to have now.
### Example
```
sudo systemctl stop hostapd
sudo systemctl start wpa_supplicant

wpa_password 5555-wombat d0a0b500 | sudo tee /etc/wpa_supplicant.conf
sudo wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &
sudo ifconfig wlan0 192.168.125.2
```
This connects to the network `5555-wombat` that has password
`d0a0b500`. Then, it sets the current wombat's ip address to
`192.168.125.2`.