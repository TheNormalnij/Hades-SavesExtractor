cmake_minimum_required(VERSION 3.20)

project(HadesSavesExtractor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if ( MSVC )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

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

add_executable(HadesSavesExtractor 
    src/SGGBinaryParser.h
    src/Hades/HadesSave.h
    src/LuaSerialize.h
    src/SGGBinaryWritter.h
    src/LuaSerialize.cpp
    src/main.cpp
    src/Hades/HadesSave.cpp
    src/adler32.h
    src/adler32.cpp
    src/Hades/HadesSaveConverter.h
    src/Hades/HadesSaveConverter.cpp
    src/GameVersions.h
)

target_link_libraries(HadesSavesExtractor PRIVATE luabins liblua lz4)

target_include_directories(HadesSavesExtractor PUBLIC
    libs/lua-5.2.2/src
    libs/luabins/submodule/src
)
