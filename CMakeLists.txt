cmake_minimum_required(VERSION 3.11)

project( LearnOpenGL )

set( GL_LIBS -lglfw -lGL -lX11 -lpthread -lXrandr -ldl )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(run)

include(src/CMakeLists.txt)

target_link_libraries( run ${GL_LIBS} )
target_include_directories( run PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_compile_options(run PUBLIC -g)
