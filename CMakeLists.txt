cmake_minimum_required(VERSION 3.10.0)
project(chess VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(chess
    main.cpp
    server.cpp
    src/Boards/board.cpp
    src/Boards/dangerMap.cpp
    src/Pieces/piece.cpp
    src/Pieces/pawn.cpp
    src/Pieces/bishop.cpp
)

target_include_directories(chess PRIVATE includes)
set_target_properties(chess PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Link Windows socket library for httplib
if(WIN32)
    target_link_libraries(chess wsock32 ws2_32)
endif()