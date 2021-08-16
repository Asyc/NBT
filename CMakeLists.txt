cmake_minimum_required(VERSION 3.21)
project(NBT)

#--------------------------------------------------------------------
# Define library and Options
#--------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)

set(HEADERS include/nbt/nbt.hpp include/nbt/nbt_type.hpp include/nbt/nbt_reader.hpp include/nbt/nbt_writer.hpp)
set(SOURCES src/nbt_type.cpp src/nbt_reader.cpp src/byteswap.hpp)

add_library(NBT ${HEADERS} ${SOURCES})

target_include_directories(NBT PRIVATE src)
target_include_directories(NBT PUBLIC include)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(NBT_STANDALONE TRUE)
endif()

option(NBT_BUILD_TESTS "Build the NBT Test Program" ${NBT_STANDALONE})

#--------------------------------------------------------------------
# Setup Endian Definition
#--------------------------------------------------------------------
include (TestBigEndian)
test_big_endian(BIG_ENDIAN)
if(BIG_ENDIAN)
    target_compile_definitions(NBT PRIVATE NBT_BIG_ENDIAN)
endif()

#--------------------------------------------------------------------
# Add Subdirectories
#--------------------------------------------------------------------
if (NBT_BUILD_TESTS)
    add_subdirectory(tests)
endif()