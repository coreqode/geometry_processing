# Geometry Processing
Non-Efficient implementation of Geometry Processing Algorithms from scratch.

---
## Supported Algorithms
### [Poisson Surface Reconstruction](https://hhoppe.com/poissonrecon.pdf)

![alt PSR](/assets/psr.png)

---
## How to use?
```
git clone --recurse-submodules https://github.com/nmwsharp/libigl-polyscope-project-template.git
cd geometry_processing
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make [exec_name] -j
./bin/[exec_name] ../data/[data_file]
```
---
## Acknowledgement
Some part of the boiler plate and helper funcions are taken from [here](https://github.com/alecjacobson/geometry-processing).


