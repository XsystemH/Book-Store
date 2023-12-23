cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(Bookstore)

add_executable(code
        Bookstore/blockchain.h
        Bookstore/storetest.cpp
        Bookstore/book.h
        Bookstore/book.cpp
        Bookstore/account.h
        Bookstore/account.cpp
        Bookstore/log.h
        Bookstore/log.cpp
)
