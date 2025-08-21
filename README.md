This repository contains the operator test code for the paper:
"A Quantum-Resistant Oracle-Based Conditional Payment Scheme from Lattice".

- The code provided here is for operators without official test implementations.
- For operators that already have official test code, please refer to the corresponding official library documentation instead.

# Experimental Environment
- Programming Language: C++17
- Compiler: gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
- OS: Ubuntu 22.04 LTS

# Instructions for Use
This library uses the [palisade-development](https://gitlab.com/palisade/palisade-development) library. Please compile this library by yourself and install the compiled package to the system. After the compilation is completed, use the following command to compile and run the code of this project.
```
// compile
mkdir -p build
cd build
cmake ..
make
// run
./{xxx}     //xxx is the executable file
```
