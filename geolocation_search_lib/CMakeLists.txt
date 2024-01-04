add_subdirectory(tests)

find_package(cpprestsdk REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PQXX REQUIRED)

add_library(geolocation_search_client
    geolocation_search.cpp
    geolocation_client.cpp
    postgresql_client.cpp
    geolocation_search_factory.cpp
    geolocation_configuration.cpp
    include/geolocation.hpp
    include/geolocation_client_interface.hpp
    include/geolocation_client.hpp
    include/geolocation_search.hpp
    include/databse_client_interface.hpp
    include/postgresql_client.hpp
    include/geolocation_search_factory.hpp
    include/null_databse.hpp
    include/geolocation_configuration.hpp)

target_include_directories(geolocation_search_client
    PUBLIC
        include
    PRIVATE
        libpqxx::pqxx_shared
        ${PQXX_INCLUDE_DIRECTORIES}
    )

target_link_libraries(geolocation_search_client
    PRIVATE
        cpprestsdk::cpprest
        ${PQXX_LIBRARIES}
    )
