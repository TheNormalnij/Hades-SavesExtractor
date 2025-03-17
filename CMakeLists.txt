cmake_minimum_required(VERSION 3.20)

project(HadesSavesExtractor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( MSVC )
    set(COMPILE_OPTIONS
        $<$<CONFIG:Release>:/Zi /W3 /O2 /MP>
        $<$<CONFIG:Debug>:/Zi /W3 /Od /GF /Gm- /RTCc /RTC1 /MP>
    )

    set(LINK_OPTIONS
        /SUBSYSTEM:WINDOWS /DYNAMICBASE:NO
        $<$<CONFIG:Release>:/INCREMENTAL:NO /OPT:REF /OPT:ICF>
        $<$<CONFIG:Debug>:/INCREMENTAL /DEBUG>
    )
endif()

add_subdirectory(libs/lua-5.2.2)
add_subdirectory(libs/luabins)

set(LZ4_BUILD_CLI OFF)
add_subdirectory(libs/lz4/build/cmake)

add_library(HadesSavesExtractorLib
    src/SGGBinaryParser.h
    src/Hades/HadesSave.h
    src/LuaSerialize.h
    src/SGGBinaryWritter.h
    src/LuaSerialize.cpp
    src/Hades/HadesSave.cpp
    src/adler32.h
    src/adler32.cpp
    src/Hades/HadesSaveConverter.h
    src/Hades/HadesSaveConverter.cpp
    src/GameVersions.h
    src/LuaFileWriteHelper.h
    src/LuaReadHelper.h
    src/App.h
    src/App.cpp
)

target_include_directories(HadesSavesExtractorLib PUBLIC ./src)
target_link_libraries(HadesSavesExtractorLib PUBLIC liblua luabins lz4)

add_executable(HadesSavesExtractor src/main.cpp)
target_link_libraries(HadesSavesExtractor PRIVATE HadesSavesExtractorLib)
set_target_properties(HadesSavesExtractor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


target_include_directories(HadesSavesExtractor PUBLIC
    libs/lua-5.2.2/src
    libs/luabins/submodule/src
)

add_executable(Hades2ParseTest tests/Hades2ParseTest.cpp)
target_link_libraries(Hades2ParseTest PRIVATE HadesSavesExtractorLib)

enable_testing()

add_test(
  NAME Hades2ParseTest
  COMMAND $<TARGET_FILE:Hades2ParseTest>
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)
