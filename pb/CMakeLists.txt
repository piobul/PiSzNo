# The library has only headers for now, so it is an INTERFACE library:
# nothing is compiled, but everyone who links it gets the include path.
# When the first .cpp file appears, change it to a normal library (see the comment below).
add_library(pb INTERFACE)
target_include_directories(pb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Example program: shows how to use the library (the course requires a main.cpp).
add_executable(pb_example examples/main.cpp)
target_link_libraries(pb_example PRIVATE pb)
target_compile_options(pb_example PRIVATE -Wall -Wextra -Wpedantic)

# Later, when src/*.cpp exist:
#   add_library(pb STATIC src/ds18b20.cpp src/bmp280.cpp src/tlv_frame.cpp)
#   target_include_directories(pb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)