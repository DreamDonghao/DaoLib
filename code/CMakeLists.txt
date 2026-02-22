cmake_minimum_required(VERSION 3.31)
if(NOT CMAKE_PROJECT_NAME)
    cmake_minimum_required(VERSION 3.31)
    project(dao LANGUAGES CXX VERSION 0.0.1)
endif()

set(PROJECT_VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_library(dao
        src/core/basic_drawing_elements/atlas_texture.cpp
        src/core/frame/app.cpp
        src/core/frame/vertex_batch_builder.cpp
        src/core/frame/window.cpp
        src/core/tool/bounding_box.cpp
        src/core/tool/mapped_region.cpp
        src/core/frame/window_controller.cpp
        src/core/frame/app_controller.cpp
        src/core/frame/context.cpp
        src/component/button/simple_button.cpp
        src/component/image.cpp
        src/interface/general_page.cpp
        src/test.cpp
)

set_target_properties(dao PROPERTIES
        DEBUG_POSTFIX "_d"
        RELEASE_POSTFIX ""
        MINSIZEREL_POSTFIX "_s"
        RELWITHDEBINFO_POSTFIX "_rd"
)

target_include_directories(dao
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)

target_link_libraries(dao
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        utf8::cpp
        utf8cpp::utf8cpp
)

install(TARGETS dao
        EXPORT daoTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT daoTargets
        FILE daoTargets.cmake
        NAMESPACE dao::
        DESTINATION lib/cmake/dao
)

include(CMakePackageConfigHelpers)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/daoConfig.cmake.in"
        [=[
@PACKAGE_INIT@

include(CMakeFindDependencyMacro)

# 列出所有 PUBLIC 依赖
find_dependency(SDL3)
find_dependency(SDL3_image)
find_dependency(SDL3_ttf)
find_dependency(utf8cpp)

include("${CMAKE_CURRENT_LIST_DIR}/daoTargets.cmake")

if(TARGET dao::dao)
    # 获取 Debug 和 Release 版本的路径
    get_target_property(_debug_loc dao::dao IMPORTED_LOCATION_DEBUG)
    get_target_property(_release_loc dao::dao IMPORTED_LOCATION_RELEASE)

    # 为缺失的配置类型设置路径
    if(_debug_loc AND NOT _release_loc)
        set_target_properties(dao::dao PROPERTIES
            IMPORTED_LOCATION_RELEASE "${_debug_loc}"
            IMPORTED_LOCATION_MINSIZEREL "${_debug_loc}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${_debug_loc}"
        )
    elseif(_release_loc AND NOT _debug_loc)
        set_target_properties(dao::dao PROPERTIES
            IMPORTED_LOCATION_DEBUG "${_release_loc}"
            IMPORTED_LOCATION_MINSIZEREL "${_release_loc}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${_release_loc}"
        )
    elseif(_release_loc AND _debug_loc)
        set_target_properties(dao::dao PROPERTIES
            IMPORTED_LOCATION_MINSIZEREL "${_release_loc}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${_release_loc}"
        )
    endif()
endif()

check_required_components(dao)
]=])

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/daoConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_BINARY_DIR}/daoConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/daoConfig.cmake"
        INSTALL_DESTINATION lib/cmake/dao
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/daoConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/daoConfigVersion.cmake"
        DESTINATION lib/cmake/dao
)

target_compile_definitions(dao
        PUBLIC
        DAO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        DAO_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        DAO_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)