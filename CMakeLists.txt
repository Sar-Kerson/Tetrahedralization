cmake_minimum_required(VERSION 2.8)
add_compile_options(-DTETLIBRARY -std=c++11 -g)
PROJECT(tetrahedron)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/core)    # dir where the cmake_list locate