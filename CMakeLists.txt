cmake_minimum_required(VERSION 3.21)
project(option_pricer LANGUAGES CXX)

add_library(option_pricer_lib
    src/options/Option.cpp
    src/options/EuropeanVanillaOption.cpp
    src/options/CallOption.cpp
    src/options/PutOption.cpp
    # autre cpp apres
)
target_include_directories(option_pricer_lib PUBLIC
    include
    include/option-pricer/options
)

add_executable(option_pricer src/main.cpp)
target_link_libraries(option_pricer PRIVATE option_pricer_lib)
enable_testing()
if (MESIFI_BUILD_TESTS)
    add_subdirectory(tests)
endif()

