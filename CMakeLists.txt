cmake_minimum_required(VERSION 3.10)
project(PolyImplBenchmark)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set path to PALISADE root (change this to your actual location)
set(PALISADE_ROOT_DIR "/usr/local/include/palisade")
set(PALISADE_BUILD_DIR "/home/lethe/palisade-development/build")

# Include PALISADE headers
include_directories(
    ${PALISADE_ROOT_DIR}
    ${PALISADE_ROOT_DIR}/core
    ${PALISADE_ROOT_DIR}/core/include
    ${PALISADE_ROOT_DIR}/pke
)

# Link PALISADE core library
link_directories(${PALISADE_BUILD_DIR}/lib)

# Add your benchmark file
add_executable(test_poly_operations test_poly_operations.cpp)
add_executable(poly_test  poly_test.cpp)
add_executable(trapdoor_test  trapdoor_test.cpp)
add_executable(GaussSampTest GaussSampTest.cpp)

# Add compiler options to all targets
foreach(target trapdoor_test)
    target_compile_options(${target} PRIVATE -g -O2)
endforeach()

# Link against PALISADE and dependencies
target_link_libraries(test_poly_operations
    PALISADEcore
    gmp
    ntl
)
target_link_libraries(poly_test
    PALISADEcore
    gmp
    ntl
)
target_link_libraries(trapdoor_test
    PALISADEcore
    PALISADEpke
)
target_link_libraries(GaussSampTest
    PALISADEcore
    PALISADEpke
)

