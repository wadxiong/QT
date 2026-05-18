cmake_minimum_required(VERSION 3.16)
project(LibraryManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
# 如果使用Qt5，请使用下面这行
# find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

qt_add_executable(LibraryManagementSystem
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    book.h
    book.cpp
    reader.h
    reader.cpp
    borrowrecord.h
    borrowrecord.cpp
    bookmanager.h
    bookmanager.cpp
    readermanager.h
    readermanager.cpp
    borrowmanager.h
    borrowmanager.cpp
)

target_link_libraries(LibraryManagementSystem PRIVATE
    Qt6::Widgets
    Qt6::Core
)
# 如果使用Qt5，请使用下面这行
# target_link_libraries(LibraryManagementSystem PRIVATE Qt5::Widgets Qt5::Core)