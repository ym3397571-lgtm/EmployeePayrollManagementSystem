cmake_minimum_required(VERSION 3.16)

project(EmployeeManagementQtWidgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(EmployeeManagementQtWidgets
    main.cpp
    mainwindow.cpp
    mainwindow.h
    data.h
)

target_link_libraries(EmployeeManagementQtWidgets PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
