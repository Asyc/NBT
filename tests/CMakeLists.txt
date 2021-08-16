message("-- [NBT] Test Building Enabled")

#--------------------------------------------------------------------
# Link against/download GTest.
#--------------------------------------------------------------------
find_package(GTest)

if (GTest_FOUND)
    set(NBT_GTEST_LIB GTest::gtest)
else ()
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    set(NBT_GTEST_LIB gtest_main)
endif ()

#--------------------------------------------------------------------
# Setup test program
#--------------------------------------------------------------------
enable_testing()

set(SOURCES reader.cpp)
add_executable(nbt_test ${SOURCES})

target_link_libraries(nbt_test NBT ${NBT_GTEST_LIB})

add_test(
        NAME nbt_test
        COMMAND nbt_test
)