cmake_minimum_required(VERSION 3.25.0)

set (CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

project(Terlang
  LANGUAGES CXX
  VERSION 0.0.1
)

file(GLOB MY_SRC "src/*.cpp")

add_executable(ter ${MY_SRC})

install(TARGETS ter DESTINATION bin)

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)
