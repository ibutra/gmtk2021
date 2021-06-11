cmake_minimum_required(VERSION 3.10)

project(GMTK)

set(CMAKE_C_STANDARD 11)

set(SRC src/main.c src/person.c)

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC inc)

add_subdirectory(lib/raylib)
target_link_libraries(${PROJECT_NAME} raylib_static)

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -O3")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()
