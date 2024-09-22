cmake_minimum_required(VERSION 3.15)

project(fcc)

aux_source_directory("${PROJECT_SOURCE_DIR}/src" FCC_ALL_SOURCE)
add_executable(fcc ${FCC_ALL_SOURCE})
set_property(TARGET fcc PROPERTY C_STANDARD 99)
target_include_directories(fcc PUBLIC "${PROJECT_SOURCE_DIR}/include")
