cmake_minimum_required(VERSION 3.25.0)

set (CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")

project(Terlang
  LANGUAGES CXX
  VERSION 0.0.1
)

add_executable(ter src/main.cpp src/Ter.cpp)

# Add sources by module
function(add_sources module)
    file(GLOB sources "src/${module}/*.cpp")
    target_sources(ter PRIVATE ${sources})
endfunction()

add_sources(tokenizer)
add_sources(utils)
add_sources(interpreter)
add_sources(parser)

install(TARGETS ter DESTINATION bin)

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)
