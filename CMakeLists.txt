cmake_minimum_required(VERSION 3.15)
project(pokemon_explorer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(pokemon_explorer 
               main.cpp 
               http/http_client.cpp
               pokemon-service/pokemon-service.cpp
               pool/thread-pool.cpp
               )

target_include_directories(pokemon_explorer PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(pokemon_explorer 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
)