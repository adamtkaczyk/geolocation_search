add_subdirectory(tests)

message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
find_package(cpprestsdk REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PQXX REQUIRED)
#find_package(libpqxx REQUIRED)

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

message("${PostgreSQL_INCLUDE_DIRS}")
target_include_directories(geolocation_search_client
    PUBLIC
        include
    PRIVATE
        libpqxx::pqxx_shared
        ${PQXX_INCLUDE_DIRECTORIES}
    )
        #${PostgreSQL_INCLUDE_DIRS})

target_link_libraries(geolocation_search_client
    PRIVATE
        cpprestsdk::cpprest
        ${PQXX_LIBRARIES}
    )
        #${PostgreSQL_LIBRARIES})
