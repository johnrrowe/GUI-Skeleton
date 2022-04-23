project(RxStreams)


cmake_minimum_required(VERSION 3.10)
set(GCC_MINIMUM_REQUIRED "11")
message(${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})


if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "Project (${PROJECT_NAME}) requires GCC version of at least ${GCC_MINIMUM_REQUIRED}")
elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_MINIMUM_REQUIRED)
        message(FATAL_ERROR "Project (${PROJECT_NAME}) requires GCC version of at least ${GCC_MINIMUM_REQUIRED}")
endif()


string(CONCAT WARNING_FLAGS 
        "-Wpedantic " 
        "-Wall "
        "-Wextra "
        "-Werror "
        "-Winit-self "
        "-Wcast-align "
        "-Wcast-qual "
        "-Wdisabled-optimization "
        "-Wlogical-op "
        "-Wmissing-declarations "
        "-Wmissing-include-dirs "
        "-Woverloaded-virtual "
        "-Wstrict-null-sentinel "
        "-Wctor-dtor-privacy "
        "-Wfloat-equal "
        "-Wredundant-decls "  
        "-Wsign-conversion "
        "-Wsign-promo " 
        "-Wstrict-overflow=5 "
        "-Wswitch-default"
)

string(CONCAT FEATURE_FLAGS 
        "-std=c++20 "
        "-fcoroutines " 
        "-fconcepts "
        "-fconcepts-diagnostics-depth=2 "
        "-fsanitize=address "
        "-O2"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${FEATURE_FLAGS}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)
