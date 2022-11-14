# BDD-Construction

This package can be used to create BDD/SDD diagrams and measure performance of different variabel reordering algorithms

## Dependencies

This project uses C++20

All used libraries are added to repository in folder `libs`
- CUDD 3.0.0
- libsdd 2.0
- spdlog
- yaml-cpp
- matplotlib-cpp
- googletest

To generate performance plots you have to uncomment lines 18-21 in CMakeLists.txt and provide valid paths to Python executable and libraries. 

## Build & Run

Run following commands to build the application:

```
mkdir build
cd build 
cmake ..
make
```

To run the program execute the binary: `./main`
Filename can be provided either via command line with `--filename` option or given in configuration file `config.yaml` along with other settings


