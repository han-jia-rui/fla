find_package(Catch2 REQUIRED)
include(Catch)

file(GLOB_RECURSE test_sources ${PROJECT_SOURCE_DIR}/test/*.cc)
add_executable(fla_test ${test_sources})
target_link_libraries(fla_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(fla_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
catch_discover_tests(fla_test)
