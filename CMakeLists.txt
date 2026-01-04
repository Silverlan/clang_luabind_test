cmake_minimum_required(VERSION 4.0)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_MODULE_STD 1)

set(TEST_CASE 0 CACHE STRING "Test Case")

project(test_program LANGUAGES CXX)

add_subdirectory(luabind)

add_executable(test_program main.cpp)

# target_compile_definitions(test_program PRIVATE "-DTEST_CASE=${TEST_CASE}")

find_package(Lua51 REQUIRED)

target_include_directories(
    test_program
    PRIVATE
	${LUA_INCLUDE_DIR}
)

target_link_libraries(test_program PRIVATE ${LUA_LIBRARIES})
target_link_libraries(test_program PUBLIC luabind)

target_compile_features(test_program PRIVATE cxx_std_23)
set_target_properties(test_program PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(test_program PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(UNIX)
    set_target_properties(test_program PROPERTIES INSTALL_RPATH "\$ORIGIN")
    target_compile_options(test_program PUBLIC -stdlib=libstdc++)
    target_link_options(test_program PUBLIC -stdlib=libstdc++)
endif()

install(TARGETS test_program DESTINATION .)
