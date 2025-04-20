# 要求C++11标准支持
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译器优化选项
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# 非Windows平台优化选项
if(NOT WIN32)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

if (POLICY CMP0060)
    cmake_policy(SET CMP0060 NEW)  # Use RUNPATH if available on the system
endif()

# 设置动态库安装路径
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

# MSVC编译器特有选项
if(MSVC)
    # 使用多线程静态库（/MT）替代/MDd或/MD
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    # set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        NXU_API_EXPORTS
    )
endif()

# 定义构建选项
option(WITH_OPENMP         "使用OpenMP"           ON )
option(BUILD_SHARED        "构建共享库"            ON)
option(BUILD_DEBUG         "构建调试版本"           OFF )
option(BUILD_TOOLS         "构建工具"              OFF )
option(USE_GPU             "使用GPU"              OFF )
option(SHOW_DEBUG_IMG      "在API内部显示调试图像"   OFF )   # API内部调试图像显示开关
option(SHOW_DEBUG_TIME     "在API内部显示处理时间"   OFF )   # API内部处理时间显示开关（BUILD_DEBUG开启时默认ON）
option(SHOW_DEBUG_INFO     "在API内部显示调试信息"   OFF )   # API内部调试信息显示开关（BUILD_DEBUG开启时默认ON)

# 根据BUILD_DEBUG选项更新API内部调试开关状态
if (BUILD_DEBUG)
    if (MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "构建类型: Release Debug" FORCE)
    endif(MSVC)
       
    option(SHOW_DEBUG_IMG  "在API内部显示调试图像"       ON )   # 显示调试图像
    option(SHOW_DEBUG_TIME "在API内部显示处理时间"       ON )   # 显示内部时间
    option(SHOW_DEBUG_INFO "在API内部显示调试信息"       ON )   # 显示内部打印
else()
    if (MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    else()
        set(CMAKE_BUILD_TYPE Release CACHE STRING "构建类型: Release Debug" FORCE)
    endif(MSVC)

    option(SHOW_DEBUG_IMG  "在API内部显示调试图像"       OFF )   # 显示调试图像
    option(SHOW_DEBUG_TIME "在API内部显示处理时间"       OFF )   # 显示内部时间
    option(SHOW_DEBUG_INFO "在API内部显示调试信息"       OFF )   # 显示内部打印
endif()

# 将公共配置的头文件配置到 include 目录下
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config/nxu_${lowercase_project_name}_gconfig.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include/nxu_${lowercase_project_name}_gconfig.h"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include")