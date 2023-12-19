cmake_minimum_required(VERSION 3.22)
project(Code)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(Bookstore)

add_executable(Book_Store
        Bookstore/blockchain.h
        Bookstore/storetest.cpp)
