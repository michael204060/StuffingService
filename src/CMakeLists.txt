cmake_minimum_required(VERSION 3.29)
project(StuffingService)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(SQLITE3_INCLUDE_DIR "C:/sqlite3")
set(SQLITE3_LIBRARIES "C:/sqlite3/sqlite3.dll")


include_directories(${SQLITE3_INCLUDE_DIR})

add_executable(StuffingService
        main.cpp
        Address.cpp
        Person.cpp
        Admin.cpp
        Specialist.cpp
        User.cpp
        Database.cpp
        MainWindow.cpp
        ../headers/Address.h
        ../headers/Admin.h
        ../headers/Database.h
        ../headers/Person.h
        ../headers/Specialist.h
        ../headers/User.h
        ../headers/MainWindow.h
        ../headers/exceptions.h
        exceptions.cpp
        ../headers/ReportGenerator.h
        ReportGenerator.cpp
        ../headers/MyContainer.h
        MyContainer.tpp
)

target_link_libraries(StuffingService
        PRIVATE Qt6::Core
        PRIVATE Qt6::Gui
        PRIVATE Qt6::Widgets
        PRIVATE ${SQLITE3_LIBRARIES})

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
