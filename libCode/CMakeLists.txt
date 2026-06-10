cmake_minimum_required(VERSION 3.31)
if (NOT CMAKE_PROJECT_NAME)
    cmake_minimum_required(VERSION 3.31)
    project(dao LANGUAGES CXX VERSION 0.0.1)
endif ()

set(PROJECT_VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif ()

if (APPLE)
    enable_language(OBJCXX)
endif ()

add_library(dao
        src/core/render/Texture.cpp
        src/core/frame/app.cpp
        src/core/render/BatchRenderer.cpp
        src/core/frame/window.cpp
        src/core/tool/BoundingBox.cpp
        src/core/tool/MappedRegion.cpp
        src/core/frame/WindowController.cpp
        src/core/frame/AppController.cpp
        src/core/frame/context.cpp
        src/interface/IGeneralPage.cpp
        src/daolib.cpp
        src/core/render/primitives/GlyphAtlas.cpp
        src/web/http/HttpTypes.cpp
        src/web/http/HttpClient.cpp
        src/web/http/HttpPoller.cpp
        src/web/http/Headers.cpp
        src/components/graphs/Arc.cpp
        src/components/graphs/Line.cpp
        src/components/graphs/Circle.cpp
        src/components/graphs/Ellipse.cpp
        src/components/graphs/Rectangle.cpp
        src/components/graphs/Ring.cpp
        src/components/graphs/RoundedRectangle.cpp
        src/components/graphs/Sector.cpp
        src/components/graphs/Triangle.cpp
)
if (APPLE)
    target_sources(dao PRIVATE src/core/frame/window_macos_helper.mm)
endif ()

target_precompile_headers(dao PRIVATE
        <httplib.h>
)

set(CMAKE_UNITY_BUILD ON)

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
find_package(httplib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
# 启用 httplib 的 OpenSSL 支持
target_compile_definitions(dao PUBLIC CPPHTTPLIB_OPENSSL_SUPPORT)

find_package(SQLiteCpp REQUIRED)

target_link_libraries(dao
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        utf8::cpp
        utf8cpp::utf8cpp
        OpenSSL::SSL
        nlohmann_json::nlohmann_json
        httplib::httplib
        OpenSSL::Crypto
        SQLiteCpp
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

# 优先查找打包在一起的 vcpkg 依赖（lib/cmake/<pkg>/）
list(PREPEND CMAKE_PREFIX_PATH "${PACKAGE_PREFIX_DIR}")

find_dependency(SDL3)
find_dependency(SDL3_image)
find_dependency(SDL3_ttf)
find_dependency(utf8cpp)
find_dependency(httplib)
find_dependency(nlohmann_json)
find_dependency(OpenSSL)
find_dependency(SQLiteCpp)

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

include("${CMAKE_CURRENT_LIST_DIR}/daoTargets.cmake")

# 补充缺失的 bzip2 静态库（freetype 依赖 bzip2，但 vcpkg wrapper 未运行时可能漏掉）
if(TARGET dao::dao)
    find_library(_dao_bz2_lib NAMES bz2 bz2d PATHS "${PACKAGE_PREFIX_DIR}/debug/lib" "${PACKAGE_PREFIX_DIR}/lib" NO_DEFAULT_PATH)
    if(_dao_bz2_lib)
        set_property(TARGET dao::dao APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${_dao_bz2_lib}")
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

# ---- 将 vcpkg 安装的依赖库、头文件、cmake 配置打包到安装目录 ----
set(_vcpkg_root "${CMAKE_BINARY_DIR}/vcpkg_installed/arm64-osx")

install(DIRECTORY "${_vcpkg_root}/lib/" DESTINATION lib)
if (EXISTS "${_vcpkg_root}/debug/lib")
    install(DIRECTORY "${_vcpkg_root}/debug/lib/" DESTINATION debug/lib)
endif ()
install(DIRECTORY "${_vcpkg_root}/include/" DESTINATION include)

# 把 vcpkg 的 cmake config (share/*) 安装到 lib/cmake 供 find_package 查找
file(GLOB _vcpkg_share_dirs LIST_DIRECTORIES true "${_vcpkg_root}/share/*")
foreach (_dir ${_vcpkg_share_dirs})
    if (IS_DIRECTORY ${_dir})
        get_filename_component(_name ${_dir} NAME)
        install(DIRECTORY "${_dir}/" DESTINATION "share/${_name}")
    endif ()
endforeach ()

target_compile_definitions(dao
        PUBLIC
        DAO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        DAO_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        DAO_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)