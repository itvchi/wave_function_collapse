Wave Function Collapse
    Implementation of the algorithm to generate map images (for further use in fpga to generate data for hdmi module)
    For testing of the final library I will use SDL library.

    Build SDL library steps:
    - cmake -S . -B build --install-prefix=$PWD/../sdllib/
    - cmake --build build --parallel $(nproc)
    - cmake --install build
    (change install path) https://stackoverflow.com/questions/6003374/what-is-cmake-equivalent-of-configure-prefix-dir-make-all-install