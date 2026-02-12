include_directories(
        ../code/include
        ../tool/example/inc
        include
)

add_executable(dao_test
        src/main.cpp
        src/atlas_region.cpp
        src/test_page.cpp
)
find_package(cpr CONFIG REQUIRED)
target_link_libraries(dao_test PRIVATE cpr::cpr)

target_link_libraries(dao_test PRIVATE dao)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(dao_test PRIVATE SDL3::SDL3)

find_package(SDL3_ttf CONFIG REQUIRED)
target_link_libraries(dao_test PRIVATE SDL3_ttf::SDL3_ttf)

add_custom_command(
        TARGET dao_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tool/example/atlas
        ${CMAKE_CURRENT_BINARY_DIR}/
)