cmake_minimum_required(VERSION 3.29)
project(AwesomeProjectPlus)

set(CMAKE_CXX_STANDARD 20)

add_executable(AwesomeProjectPlus
        main.cpp
        Address.cpp
        Person.cpp
        Admin.cpp
        Specialist.cpp
        User.cpp
        headers/Person.h
        headers/Address.h
        headers/Admin.h
        headers/User.h
        headers/Specialist.h
        )
