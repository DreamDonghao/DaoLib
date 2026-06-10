#!/bin/bash
set -euo pipefail

# ============================================================
# DaoLib 构建 & 安装脚本
#
# 用法:
#   ./build-install.sh              # 构建 Debug + Release（增量）
#   ./build-install.sh Debug        # 仅构建 Debug
#   ./build-install.sh Release      # 仅构建 Release
#   ./build-install.sh Debug 1      # 清理后重建 Debug
#   ./build-install.sh Release 1    # 清理后重建 Release
# ============================================================

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_TYPE="${1:-all}"
CLEAN="${2:-0}"

VCPKG_TOOLCHAIN="${VCPKG_TOOLCHAIN:-$HOME/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake}"
NPROC="$(sysctl -n hw.logicalcpu 2>/dev/null || nproc)"

if [[ "${BUILD_TYPE}" == "all" ]]; then
    TYPES=("Debug" "Release")
else
    TYPES=("${BUILD_TYPE}")
fi

# ---- 颜色 ----
RED='\033[0;31m'; GREEN='\033[0;32m'; CYAN='\033[0;36m'; NC='\033[0m'
step()  { echo -e "${CYAN}[${1}]${NC} ${2}"; }
ok()    { echo -e "${GREEN}  ✓${NC} ${1}"; }

echo "========================================"
echo " DaoLib 构建安装脚本"
echo "========================================"
echo " 项目目录:   ${PROJECT_DIR}"
echo " 并行任务:   ${NPROC}"
echo " 构建类型:   ${TYPES[*]}"
echo "========================================"

# ---- 清理 ----
if [[ "${CLEAN}" == "1" ]]; then
    step "0/3" "清理所有旧构建 & 安装..."
    rm -rf "${PROJECT_DIR}/cmake-build-debug" "${PROJECT_DIR}/cmake-build-release" "${PROJECT_DIR}/install"
fi

# ---- CMake 共用参数 ----
CMAKE_ARGS=(
    -G Ninja
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_TOOLCHAIN}"
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)

for TYPE in "${TYPES[@]}"; do
    _type_lower="$(echo "${TYPE}" | tr '[:upper:]' '[:lower:]')"
    BUILD_DIR="${PROJECT_DIR}/cmake-build-${_type_lower}"
    INSTALL_DIR="${PROJECT_DIR}/install/${TYPE}"

    # 非 Ninja 缓存则清理
    if [[ -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        _gen="$(grep CMAKE_GENERATOR:INTERNAL "${BUILD_DIR}/CMakeCache.txt" 2>/dev/null | cut -d= -f2)"
        [[ "${_gen}" != "Ninja" ]] && rm -rf "${BUILD_DIR}"
    fi

    echo ""
    echo -e "${GREEN}═══ ${TYPE} ═══${NC}"

    step "1/3" "CMake 配置..."
    cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" \
        -DCMAKE_BUILD_TYPE="${TYPE}" \
        -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
        "${CMAKE_ARGS[@]}"

    step "2/3" "编译 dao..."
    cmake --build "${BUILD_DIR}" --target dao -j "${NPROC}"

    step "3/3" "安装 → ${INSTALL_DIR}"
    cmake --install "${BUILD_DIR}"
done

# ---- 结果 ----
echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN} 全部构建完成!${NC}"
echo -e "${GREEN}========================================${NC}"
for TYPE in "${TYPES[@]}"; do
    echo "  [${TYPE}] ${PROJECT_DIR}/install/${TYPE}"
done
echo "========================================"
echo ""
echo "在其他项目中使用:"
echo ""
echo "  cmake -S <src> -B <build> \\"
echo "    -DCMAKE_PREFIX_PATH=\"${PROJECT_DIR}/install/Release\""
echo ""
echo "  CMakeLists.txt:"
echo "    find_package(dao REQUIRED)"
echo "    target_link_libraries(myapp PRIVATE dao::dao)"