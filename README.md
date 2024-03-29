# IHS Boost
![CMake Build](https://github.com/ihsrobotics/ihsboost/actions/workflows/cmake.yml/badge.svg)

The ihs library of robot functions, from steady acceleration to servo movement
## Installing
### Dependencies
Some ihsboost modules have dependencies. The format below is dependency - modules - installation instructions
* libwallaby - servos, movement, create_extra - please use [this branch](https://github.com/chrehall68/libwallaby/tree/refactor) to install libwallaby
* libjsoncpp - util - on linux, run `sudo apt-get install libjsoncpp-dev` to install
* libbluetooth - communicate - on linux, run `sudo apt-get install libbluetooth-dev` to install
* libboost_python - bindings - on linux, run `sudo apt-get install libboost-python-dev` to install
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
# important: do not use the -j4 flag when building on wombat
make -j4
sudo make install
```
**If you plan on using the python bindings, run `sudo ldconfig` to index the ihsboost shared objects.**

Note: you will need to use either the `main` branch or the 
`older-wombat` branch depending on whether you are using a
new wombat or an old (original OS) wombat (see `Getting the Source files`) 
### Cross Compile
In order to cross-compile ihsboost, make sure to create a toolchain file. It should look
something like this
```cmake
# set the target architecture and system name (for raspberry pi, is aarch64)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# set the compiler you want to use
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# location of libs and includes to link/compile against
set(CMAKE_SYSROOT /path/to/your/sysroot)

# when using clang, make sure to set the c and cxx flags (cpu is optional)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 --target=${CMAKE_SYSTEM_PROCESSOR}-linux-gnu --sysroot=${CMAKE_SYSROOT} -mcpu=cortex-a53")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

# when building a debian package, make sure to set this variable
# to the output of `dpkg --print-architecture` on the target machine
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)

# where you want to install the files locally before copying
# them to their final destination
set(CMAKE_STAGING_PREFIX /path/to/temporary/out)
```
Then, just specify the toolchain file when configuring with the option
`-DCMAKE_TOOLCHAIN_FILE=/path/to/your/toolchain/file`. For example, to
cross compile the full library, you could run
```shell
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/path/to/your/toolchain/file`
```
After configuring with cmake, building can proceed normally
### Other Build Options
The following are options that can be appended to the `cmake` command
when configuring the project.

* `-Dbuild_library=ON/OFF` - build ihsboost, defaults to `ON`
* `-Dbuild_tests=OFF/ON` - build tests to assure that ihsboost built successfully, defaults to `OFF`
* `-Dbuild_debian=OFF/ON` - build debian package for easy install / uninstall
* `-Dwith_documentation=OFF/ON` - build documentation files for the project, defaults to `OFF`
* `-Droomba=ON/OFF` - use roomba configs or not, defaults to `ON`
* `-Dpython_version=XXX` - use a specific version of python (for example, 3.9 or 3.10), defaults to 3.9,
only relevant if building python bindings

The following are options to configure which ihsboost modules to build
* `-Dbuild_bindings=ON/OFF`- build python bindings, defaults to `ON`
* `-Dbuild_communicate=ON/OFF` - build communicate, defaults to `ON`
* `-Dbuild_controllers=ON/OFF` - build controllers, defaults to `ON`
* `-Dbuild_create_extra=ON/OFF` - build extra create functionality (vacuum and brushes), defaults to `ON`
* `-Dbuild_movement=ON/OFF` - build movement functions, defaults to `ON`
* `-Dbuild_servos=ON/OFF` - build servo movement functions, defaults to `ON`
* `-Dbuild_thread=ON/OFF` - build threading classes, defaults to `ON`
* `-Dbuild_util=ON/OFF` - build util, defaults to `ON`

For example,
`cmake .. -DCMAKE_BUILD_TYPE=Release -Dwith_documentation=ON -Dpython_version=3.10`
will configure cmake to build the library and python bindings 
(since `build_library` and `build_bindings` default to `ON`) but
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
