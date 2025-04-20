cmake_minimum_required(VERSION 3.10)

# 确保lowercase_project_name已定义
if(NOT DEFINED lowercase_project_name)
	message("Variable 'lowercase_project_name' is not defined. Please set it before proceeding.")
	return()
endif()

# 设置源文件路径（示例，根据实际情况调整）
set(SRC_DIR src)
set(INCLUDE_DIR include)

# 显式列出源文件
set(libSRCS
    ${SRC_DIR}/nxu_${lowercase_project_name}_api.cpp
    ${SRC_DIR}/nxu_${lowercase_project_name}_utils.cpp
)

# 添加库
if(BUILD_SHARED)
	add_library(${LIBRARY_OUTPUT_NAME} SHARED ${libSRCS})
else()
	add_library(${LIBRARY_OUTPUT_NAME} STATIC ${libSRCS})
endif()

# 使用target_include_directories管理目标依赖的头文件路径
target_include_directories(${LIBRARY_OUTPUT_NAME}
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR}>
	INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR}>
	$<INSTALL_INTERFACE:include>
)

if(UNIX AND NOT APPLE) # Linux特有的设置
	set_property(TARGET ${LIBRARY_OUTPUT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON) # 为共享库开启PIC
	set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

# 添加OpenCV链接库到你的目标
target_link_libraries(${LIBRARY_OUTPUT_NAME} PUBLIC ${EXTRA_LIBS})

# 安装头文件到include目录
install(FILES
	${INCLUDE_DIR}/nxu_${lowercase_project_name}_api.h
	${INCLUDE_DIR}/nxu_common.h
	DESTINATION include
	COMPONENT Development
)

# 安装库文件到常规位置（用于发布）
install(TARGETS ${LIBRARY_OUTPUT_NAME}
	RUNTIME DESTINATION lib COMPONENT Release_RuntimeLibraries # 如果有可执行文件也在这里指定
	LIBRARY DESTINATION lib COMPONENT Release_RuntimeLibraries
	ARCHIVE DESTINATION lib COMPONENT Release_Development
	INCLUDES DESTINATION include
)

# 安装库文件到tools目录下（用于测试）
install(TARGETS ${LIBRARY_OUTPUT_NAME}
	RUNTIME DESTINATION tools COMPONENT Test_RuntimeLibraries # 如果有可执行文件也在这里指定
)
