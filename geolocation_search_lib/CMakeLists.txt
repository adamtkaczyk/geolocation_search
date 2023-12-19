# cmake_minimum_required(VERSION 3.27)

# set(CMAKE_CXX_STANDARD 17)

# project(geolocation_search_client)

#enable_testing()

add_subdirectory(tests)

find_package(cpprestsdk REQUIRED)

add_library(geolocation_search_client
    geolocation_search.cpp
    geolocation_client.cpp
    postgresql_client.cpp
    include/geolocation.hpp
    include/geolocation_client_interface.hpp
    include/geolocation_client.hpp
    include/geolocation_search.hpp
    include/databse_client_interface.hpp
    include/postgresql_client.hpp
    include/geolocation_search_factory.hpp
    geolocation_search_factory.cpp
    include/null_databse.hpp)
target_include_directories(geolocation_search_client PUBLIC include)

target_link_libraries(geolocation_search_client
    PRIVATE
    cpprestsdk::cpprest)