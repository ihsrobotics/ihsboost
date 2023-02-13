# IHS Boost
The ihs library of robot functions, from steady acceleration to servo movement
## Installing
### Getting the Source files
To get the source files on a wombat, the best way to do that is to start on your local
computer. Run `git clone https://github.com/ihsrobotics/ihsboost.git` to get the github
repository locally.

If you are going to install on an older wombat, make sure to run the following in the
same directory on your local machine:
```
cd ./ihsboost
git switch older-wombat
cd ..
```

Finally, copy the directory over with the following command:
`scp -r ./ihsboost pi@(ipaddress):~/`
where `(ipaddress)` should be the ip addresss (ie 192.168.125.1). This
will copy ihsboost into the home directory of the wombat.
### Wombat Build
To build on the wombat, run the following commands
from terminal inside the ihsboost directory on the wombat
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/
make -j4
sudo make install
```
Note: you will need to use either the `main` branch or the 
`older-wombat` branch depending on whether you are using a
new wombat or an old (original OS) wombat (see `Getting the Source files`) 
### Cross Compile Build
Currently, Cross compile build is unsupported.
## Compiling programs with it
To compile a program with the ihsboost library, then
assuming that you have already installed ihsboost,
all you have to do is the following command to compile
to form the executable `./a.out`:
```
g++ (file) -lihsboost -lkipr -pthread -rt -std=c++11
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

wpa_passphrase (wifi_name) (wifi_password) | sudo tee /etc/wpa_supplicant.conf
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

wpa_passphrase 5555-wombat d0a0b500 | sudo tee /etc/wpa_supplicant.conf
sudo wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &
sudo ifconfig wlan0 192.168.125.2
```
This connects to the network `5555-wombat` that has password
`d0a0b500`. Then, it sets the current wombat's ip address to
`192.168.125.2`.
## Python Communicate Bindings
To build the python communicate bindings, first create `user-config.jam` in the home directory (`~/`). It should have the following contents:
```
using python
    : 3.9
    : /usr/bin/python3.9
    : /usr/include/python3.9
    : /usr/lib/python3.9 ;
```
Next, we need to create a symlink:
```
sudo ln --symbolic /usr/lib/aarch64-linux-gnu/libboost_python39.so /usr/lib/libboost_python.so
```

Next, enter the bind directory in ihsboost
and run `bjam`. This will build the python bindings, which can
be imported in a python file by importing the module `ihs_communicate`.

Finally, to "install" this python module, run the following command:
```
sudo cp ./ihs_communicate.cpython*.so /usr/local/python3.9/dist-packages/
```

Note: you can change the python version for by changing all the `3.9`'s and `39`'s to whatever version of python you have