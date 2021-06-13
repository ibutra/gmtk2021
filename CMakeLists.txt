cmake_minimum_required(VERSION 3.10)

project(CoupleJoin)

set(CMAKE_C_STANDARD 11)

set(SRC src/main.c src/person.c src/personarray.c src/game.c src/gui_helper.c src/mainmenu.c src/names.c src/font.c src/creditscreen.c)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX_BUNDLE_ICON_FILE logo.icns)
    set(myIcon data/logo.icns)
    set_source_files_properties(${myIcon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${myIcon} ${SRC})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_executable(${PROJECT_NAME} WIN32 ${SRC})
else()
    add_executable(${PROJECT_NAME} ${SRC})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC inc data lib/raygui)

add_subdirectory(lib/raylib)
target_link_libraries(${PROJECT_NAME} raylib_static)

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -O3 --shell-file ../web/shell_minimal.html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()
