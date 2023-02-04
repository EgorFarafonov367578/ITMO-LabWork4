cmake_minimum_required(VERSION 3.23)
project(labwork_4_EgorFarafonov367578)

set(CMAKE_CXX_STANDARD 17)

add_executable(labwork_4_EgorFarafonov367578 main.cpp
        Ham/HammingArchive.cpp
        Ham/HammingArchive.h
        Ham/HamCore/HamReader.cpp
        Ham/HamCore/HamReader.h
        Ham/HamCore/HamWriter.cpp
        Ham/HamCore/HamWriter.h)