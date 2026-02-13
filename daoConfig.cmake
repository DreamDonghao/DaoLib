include(CMakeFindDependencyMacro)

find_dependency(SDL3)
find_dependency(SDL3_image)
find_dependency(SDL3_ttf)
find_dependency(utf8cpp)

include("${CMAKE_CURRENT_LIST_DIR}/daoTargets.cmake")
