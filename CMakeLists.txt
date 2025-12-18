cmake_minimum_required(VERSION 4.0)
project(Lab_3)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest CONFIG REQUIRED)

add_executable(Lab_3
        src/main.cpp
        src/ConfigParser.cpp
        src/ConfigParser.h
        src/Converter.h
        src/Factory.h
        Converters/Mixer_Converter.cpp
        Converters/Mixer_Converter.h
        Converters/Mute_Converter.cpp
        Converters/Mute_Converter.h
        Converters/Bass_Boost.cpp
        Converters/Bass_Boost.h
        src/WAV_Reader.cpp
        src/WAV_Reader.h
        src/WAV_Writer.cpp
        src/WAV_Writer.h
        src/convert.cpp
        src/convert.h)


add_executable(Lab_3_test
        test/test.cpp
        src/ConfigParser.cpp
        src/ConfigParser.h
        src/Converter.h
        src/Factory.h
        Converters/Mixer_Converter.cpp
        Converters/Mixer_Converter.h
        Converters/Mute_Converter.cpp
        Converters/Mute_Converter.h
        Converters/Bass_Boost.cpp
        Converters/Bass_Boost.h
        src/WAV_Reader.cpp
        src/WAV_Reader.h
        src/WAV_Writer.cpp
        src/WAV_Writer.h
        src/convert.cpp
        src/convert.h)

target_link_libraries(Lab_3_test
        GTest::gtest_main
)

include(CTest)
add_test(NAME run_tests COMMAND Lab_3_test)