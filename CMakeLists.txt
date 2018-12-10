cmake_minimum_required(VERSION 3.0)
INCLUDE_DIRECTORIES(includes)
file(GLOB genlang_src
        "src/*cpp"
        )

add_executable(genlang ${genlang_src})
