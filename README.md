# IHS Boost
![CMake Build](https://github.com/ihsrobotics/ihsboost/actions/workflows/cmake.yml/badge.svg)

The ihs library of robot functions, from steady acceleration to servo movement
## Installing
### Dependencies
ihsboost depends on the following libraries:
* libwallaby - please use [this branch](https://github.com/chrehall68/libwallaby/tree/refactor) to install libwallaby
* libjsoncpp - on linux, run `sudo apt-get install libjsoncpp-dev` to install
### Getting the Source files
To get the source files on a wombat, the best way to do that is to start on your local
computer. Run `git clone https://github.com/ihsrobotics/ihsboost.git` to get the github
repository locally.

If you are going to install on an older wombat, make sure to run the following in the
same directory on your local machine:

```shell
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

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
sudo make install
```

Note: you will need to use either the `main` branch or the 
`older-wombat` branch depending on whether you are using a
new wombat or an old (original OS) wombat (see `Getting the Source files`) 
### Cross Compile Build
Currently, Cross compile build is unsupported.
### Other Build Options
The following are options that can be appended to the `cmake` command
when configuring the project.

* `-Dwith_documentation=OFF/ON` - build documentation files for the project, defaults to `OFF`
* `-Dbuild_library=ON/OFF` - build ihsboost, defaults to `ON`
* `-Dbuild_python=ON/OFF`- build python bindings, defaults to `ON`
* `-Dpython_version=XXX` - use a specific version of python (for example, 3.9 or 3.10), defaults to 3.9

For example,
`cmake .. -DCMAKE_BUILD_TYPE=Release -Dwith_documentation=ON -Dpython_version=3.10`
will configure cmake to build the library and python bindings 
(since `build_library` and `build_python` default to `ON`) but
will also make the documentation (since `with_documentation` was specified to `ON`)
and use python 3.10 instean of python 3.9
## Compiling programs with it
To compile a program with the ihsboost library, then
assuming that you have already installed ihsboost,
all you have to do is the following command to compile
to form the executable `./a.out`:

```shell
g++ (file) -lihsboost -lkipr -pthread -lrt -std=c++11
```

Note: `(file)` should be replaced by the name of the file that
should be compiled.

Note: `-lrt` is only necessary on older wombats
## Running python programs with it
To run a python program using ihsboost, import the python
module `ihs_bindings` in your program. An example program might
look like this:

```python
import ihs_bindings
print(dir(ihs_bindings))
```

## Network Config
### Steps
If you are trying to connect from older wombat to older wombat, then
the following extra steps are required from one of them in order
to connect to the wifi of another wombat:

```shell
sudo systemctl stop hostapd
sudo systemctl start wpa_supplicant

wpa_passphrase (wifi_name) (wifi_password) | sudo tee /etc/wpa_supplicant.conf
```

Where `(wifi_name)` is the name of the wifi that you want to connect to
and `(wifi_password)` is the password for that wifi network.

Then, in a terminal that you will keep open, run

```shell
sudo wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &
sudo ifconfig wlan0 (new_ip_address)
```

Where `(new_ip_address)` is the ip address that you want this wombat
to have now.
### Example
```shell
sudo systemctl stop hostapd
sudo systemctl start wpa_supplicant

wpa_passphrase 5555-wombat d0a0b500 | sudo tee /etc/wpa_supplicant.conf
sudo wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &
sudo ifconfig wlan0 192.168.125.2
```

This connects to the network `5555-wombat` that has password
`d0a0b500`. Then, it sets the current wombat's ip address to
`192.168.125.2`.
