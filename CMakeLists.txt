cmake_minimum_required(VERSION 3.17)

project(BigInteger CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BigInteger)


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(BigInteger PUBLIC -fconstexpr-ops-limit=1243554432)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(BigInteger PUBLIC -fconstexpr-steps=1243554432)
endif()

target_compile_options(BigInteger PRIVATE -Wall -Wextra -pedantic -Werror)
#target_compile_options(BigInteger PUBLIC -stdlib=libc++)

target_sources(BigInteger PRIVATE
	biginteger.h
	)

target_include_directories(BigInteger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(BigInteger PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(tests)
