cmake_minimum_required (VERSION 3.10) # 设置支持的CMake最小版本

project(BSD) # 定义项目名称

# 版本信息及签名信息
# 包含 version.cmake 文件，该文件负责获取Git提交信息、设置项目版本号、生成签名信息等
include(cmake/version.cmake)

# 设置编译器选项与平台特定配置
# 包含 platform_compiler.cmake 文件，该文件定义了项目所需的编译器选项（如C++标准、编译器优化），
# 同时处理特定平台（如Windows与非Windows）的编译器选项、宏定义等
include(cmake/platform_compiler.cmake)

# 找到相关的依赖
# 包含 find_depends.cmake 文件，该文件负责查找并设置项目所需的外部依赖库
include(cmake/find_depends.cmake)

# 定义库类型和命名规则
if(BUILD_SHARED)
    set(LIBRARY_TYPE SHARED)
    set(LIBRARY_SUFFIX "")
else()
    set(LIBRARY_TYPE STATIC)
    set(LIBRARY_SUFFIX "_static")
endif()

if(BUILD_DEBUG)
    set(LIBRARY_DEBUG_SUFFIX "d")
endif()

if(USE_GPU)
    set(LIBRARY_GPU_SUFFIX "_gpu")
else()
    set(LIBRARY_GPU_SUFFIX "_cpu")
endif()

set(LIBRARY_NAME ${lowercase_project_name})
set(LIBRARY_OUTPUT_NAME ${LIBRARY_NAME}${LIBRARY_SUFFIX}${LIBRARY_DEBUG_SUFFIX}${LIBRARY_GPU_SUFFIX})


# 设置安装目录
# 根据平台（Win32 或其他）设置不同的安装目录
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "E:/build/nxu_solution") # Win32平台下安装目录
else()
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/install/nxu_solution") # 非Win32平台下安装目录
endif()


# 针对不同子模块的处理
# 添加 'source' 子目录，其中包含项目的源代码和相关CMake配置
add_subdirectory(source)

# # 添加工具编译（如果 BUILD_TOOLS 选项开启）
if (BUILD_TOOLS)
add_subdirectory(tools)
endif(BUILD_TOOLS)

# 输出构建相关信息
message(STATUS ">>>>>>>>>>>>>>>>>>>> message start  >>>>>>>>>>>>>>>>>>>>>>>>>>")
message(STATUS "> Project Version: ${PROJECT_VERSION}")
message(STATUS "> Git Commit MD5 : ${GIT_COMMIT_MD5}") # 输出项目版本信息与Git提交哈希
message(STATUS "> CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}") # 输出构建类型
message(STATUS "> Building at ${BUILDDATETIME}") # 输出构建时间
message(STATUS "> CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}") # 输出安装目录
message(STATUS "> BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}") # 输出是否构建共享库
message(STATUS "<<<<<<<<<<<<<<<<<<<< message end <<<<<<<<<<<<<<<<<<<<<<<<<<<<")
