cmake_minimum_required(VERSION 3.29)
project(AwesomeProjectPlus)

set(CMAKE_CXX_STANDARD 20)
set(SQLITE3_INCLUDE_DIR "C:/sqlite3")
set(SQLITE3_LIBRARIES "C:/sqlite3/sqlite3.dll")

include_directories(headers ${SQLITE3_INCLUDE_DIR}) # Include headers directory

add_executable(AwesomeProjectPlus
        main.cpp
        Address.cpp
        Person.cpp
        Admin.cpp
        Specialist.cpp
        User.cpp
) # Only .cpp files here

target_link_libraries(AwesomeProjectPlus PRIVATE ${SQLITE3_LIBRARIES})