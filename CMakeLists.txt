cmake_minimum_required(VERSION 3.4.3)
project(simple_calculator)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

#add_executable(simple-tool test_llvm.cpp)
#target_link_libraries(simple-tool LLVM-6.0)

add_executable(simple_calculator simple_calculator.cpp)
add_executable(calculator_llvmIR calculator_llvmIR.cpp)