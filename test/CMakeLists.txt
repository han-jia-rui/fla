find_package(Catch2 REQUIRED)
include(Catch)

file(GLOB_RECURSE test_sources ${PROJECT_SOURCE_DIR}/test/*.cc)
file(GLOB_RECURSE fla_sources ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(fla_test
                    ${test_sources}
                    ${PROJECT_SOURCE_DIR}/src/simulator.cc
                    ${PROJECT_SOURCE_DIR}/src/pda.cc
                    ${PROJECT_SOURCE_DIR}/src/pda_parser.cc
                    ${PROJECT_SOURCE_DIR}/src/tm.cc
                    ${PROJECT_SOURCE_DIR}/src/tm_parser.cc
)

target_link_libraries(fla_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(fla_test
                            PUBLIC ${PROJECT_SOURCE_DIR}/include
                            PRIVATE ${PROJECT_SOURCE_DIR}/src)
catch_discover_tests(fla_test)
