cmake_minimum_required(VERSION 3.10.0)
project(chess VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(chess
    main.cpp
    src/Boards/board.cpp
    src/Pieces/piece.cpp
    src/Pieces/pawn.cpp
)

target_include_directories(chess PRIVATE includes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)