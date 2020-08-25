project(BigInteger CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BigInteger)

add_compile_options(-Wall -Wextra -pedantic -Werror -fconstexpr-ops-limit=243554432)

target_sources(BigInteger PRIVATE
	biginteger.h
	)

target_include_directories(BigInteger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(BigInteger PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
