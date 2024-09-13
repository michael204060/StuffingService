cmake_minimum_required(VERSION 3.29)
project(AwesomeProjectPlus)

set(CMAKE_CXX_STANDARD 20)

add_executable(AwesomeProjectPlus main.cpp
        Address.cpp
        Address.h
        Person.cpp
        Person.h
        Admin.cpp
        Admin.h
        Specialist.cpp
        Specialist.h
        User.cpp
        User.h)
