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