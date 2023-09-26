cmake_minimum_required (VERSION 3.12)

project (luaDraw)

set (MY_OPENGL_WRAPPER_TEST FALSE)
add_subdirectory (myOpenglWrapper)

find_package (Lua REQUIRED)

add_subdirectory (src)
