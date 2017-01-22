cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++11 -g) # Use debug simbols (-g). Can work without, but I dare you try to read the stack traces.

add_executable(overridenew main.cpp SomeClass.cpp InstrumentedNew.cpp)

