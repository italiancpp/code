cmake_minimum_required(VERSION 2.6)

if( NOT UNIX )
  message( FATAL_ERROR "This code is not portable. Works only on Linux." )
endif()

add_definitions(-std=c++11 -g) # Use debug simbols (-g). Can work without, but I dare you try to read the stack traces then.

add_executable(overridenew main.cpp SomeClass.cpp InstrumentedNew.cpp)

