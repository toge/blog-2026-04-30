cmake_minimum_required(VERSION 3.25)
project(blog-2026-04-30 LANGUAGES CXX)

## add warning
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
endif()

if (MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

## optimization
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
endif()

## other flag
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ctre CONFIG REQUIRED)
find_package(pcre2 CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(oniguruma CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS i18n uc data)
find_package(unofficial-tre CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex)
find_path(SRELL_INCLUDE_DIRS "srell.hpp")

## add executable
add_executable(std_regex std_regex.cpp)
target_compile_features(std_regex PRIVATE cxx_std_23)
target_include_directories(std_regex PRIVATE .)

add_executable(boost_regex boost_regex.cpp)
target_link_libraries(boost_regex PRIVATE Boost::regex)
target_compile_features(boost_regex PRIVATE cxx_std_23)
target_include_directories(boost_regex PRIVATE .)

add_executable(ctre ctre.cpp)
target_link_libraries(ctre PRIVATE ctre::ctre)
target_compile_features(ctre PRIVATE cxx_std_23)
target_include_directories(ctre PRIVATE .)

add_executable(pcre2 pcre2.cpp)
target_link_libraries(pcre2 PRIVATE PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX)
target_compile_features(pcre2 PRIVATE cxx_std_23)
target_include_directories(pcre2 PRIVATE .)

add_executable(pcre2_jit pcre2.cpp)
target_compile_definitions(pcre2_jit PRIVATE PCRE2_JIT)
target_link_libraries(pcre2_jit PRIVATE PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX)
target_compile_features(pcre2_jit PRIVATE cxx_std_23)
target_include_directories(pcre2_jit PRIVATE .)

add_executable(re2 re2.cpp)
target_link_libraries(re2 PRIVATE re2::re2)
target_compile_features(re2 PRIVATE cxx_std_23)
target_include_directories(re2 PRIVATE .)

add_executable(oniguruma oniguruma.cpp)
target_link_libraries(oniguruma PRIVATE oniguruma::onig)
target_compile_features(oniguruma PRIVATE cxx_std_23)
target_include_directories(oniguruma PRIVATE .)

add_executable(icu icu.cpp)
target_link_libraries(icu PRIVATE ICU::i18n ICU::uc ICU::data)
target_compile_features(icu PRIVATE cxx_std_23)
target_include_directories(icu PRIVATE .)

add_executable(tre tre.cpp)
target_link_libraries(tre PRIVATE unofficial::tre::tre)
target_compile_features(tre PRIVATE cxx_std_23)
target_include_directories(tre PRIVATE .)

add_executable(srell srell.cpp)
target_include_directories(srell PRIVATE ${SRELL_INCLUDE_DIRS})
target_compile_features(srell PRIVATE cxx_std_23)
target_include_directories(srell PRIVATE .)
