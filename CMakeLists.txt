project(BigInteger CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BigInteger)

target_compile_options(BigInteger PUBLIC -fconstexpr-ops-limit=1243554432)
target_compile_options(BigInteger PRIVATE -Wall -Wextra -pedantic -Werror)

target_sources(BigInteger PRIVATE
	biginteger.h
	)

target_include_directories(BigInteger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(BigInteger PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
