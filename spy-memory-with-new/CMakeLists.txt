cmake_minimum_required(VERSION 2.6)

if( NOT UNIX )
  message( FATAL_ERROR "This code is not portable. Works only on Linux." )
endif()

find_package(Boost)
if (NOT Boost_FOUND)
  message( FATAL_ERROR "Install Boost, or remove the line with boost::share_ptr from main.cpp." )
endif()

add_definitions(-std=c++11 -g) # Use debug simbols (-g). Can work without, but I dare you try to read the stack traces then.

add_executable(overridenew main.cpp SomeClass.cpp InstrumentedNew.cpp)

