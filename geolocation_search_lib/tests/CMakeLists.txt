add_executable(geolocation_search_tests
    main.cpp
    geolocation_search_client_tests.cpp
    http_test_server.hpp
    http_test_server.cpp)

find_package(GTest REQUIRED)
target_link_libraries(geolocation_search_tests
 PRIVATE
  GTest::GTest
  geolocation_search_client)

add_test(geolocation_search_tests geolocation_search_client)
