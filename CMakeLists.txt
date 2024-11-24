cmake_minimum_required(VERSION 3.29)
project(Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project
    2048.h
    AccountManage.h
    CryptoUtils.h
    DataStruct.h
    Game.h
    LogIn.h
    main.cpp
    Maze.h
    MenuUI.h
    Moduel.h
    Person.h
    Sign.h
    Utils.h)
