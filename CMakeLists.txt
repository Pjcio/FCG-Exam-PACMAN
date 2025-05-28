cmake_minimum_required(VERSION 3.22)
project(FCGPACMAN)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


function(add_tappa tappa_num)
    set(tappa_name "Tappa${tappa_num}")
    file(GLOB_RECURSE source_files "${tappa_name}/*.cpp")

    add_executable(${tappa_name} ${source_files})
    target_include_directories(${tappa_name} PRIVATE "${tappa_name}")

    target_link_libraries(${tappa_name} PRIVATE SFML::Graphics sfml-audio)
endfunction()

add_tappa(00)

if(EXISTS "${CMAKE_SOURCE_DIR}/resources")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/resources")
    
    add_custom_command(TARGET Tappa00 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/resources"
            "${CMAKE_BINARY_DIR}/resources"
        COMMENT "Copying resources directory"
    )
endif()

foreach(tappa_num RANGE 1 9)
    add_tappa(0${tappa_num})
endforeach()