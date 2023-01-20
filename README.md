# IHS Boost
The ihs library of robot functions, from steady acceleration to servo movement
## Installing
### Wombat Build
To build on the wombat, run the following commands
from terminal inside the ihsboost directory
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
sudo make install
```
### Cross Compile Build
To build for the wombat on your machine, then
run the following commands inside the ihsboost directory

If you are targeting aarch64
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ -DCMAKE_INSTALL_PREFIX=../out
make -j4
```

If you are targeting arm
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=arm-none-eabi-g++ -DCMAKE_INSTALL_PREFIX=../out
make -j4
```

Next, copy the out directory to the wombat.
Once there, copy the include files to /usr/local/include and
copy the library to /usr/local/lib