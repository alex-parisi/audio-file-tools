cmake_minimum_required(VERSION 3.29)
project(audio-file-tools VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Wav Read/Write Test
set(WAV_TEST_SOURCES
        src/WavUtils.cpp
        src/WavReader.cpp
        src/WavWriter.cpp
        test/WavWriterTest.cpp
)
add_executable(WavTest ${WAV_TEST_SOURCES})
target_include_directories(WavTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(WavTest PRIVATE
        GTest::gtest_main
)
target_compile_options(WavTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(WavTest PRIVATE
        -fsanitize=address
)
set_target_properties(WavTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_WavTest COMMAND WavTest)