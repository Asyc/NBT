cmake_minimum_required(VERSION 3.21)
project(NBT)

set(CMAKE_CXX_STANDARD 17)
include (TestBigEndian)



set(HEADERS include/nbt/nbt.hpp include/nbt/nbt_type.hpp include/nbt/nbt_reader.hpp src/nbt_reader.cpp)
set(SOURCES src/nbt_type.cpp src/byteswap.hpp)

add_library(NBT ${HEADERS} ${SOURCES})

target_include_directories(NBT PRIVATE src)
target_include_directories(NBT PUBLIC include)
test_big_endian(BIG_ENDIAN)
if(BIG_ENDIAN)
    target_compile_definitions(NBT PUBLIC NBT_BIG_ENDIAN)
endif()


find_package(GTest)

if (GTest_FOUND)
    enable_testing()

    add_subdirectory(tests)
    message("-- [NBT] Test Building Enabled")
endif()