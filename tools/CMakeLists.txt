# 生成应用程序
add_executable(${lowercase_project_name}_tools_sel ${lowercase_project_name}_tools_sel.cpp OutputFileManager.cpp TargetDetector.cpp)

# 链接库
target_link_libraries(${lowercase_project_name}_tools_sel ${LIBRARY_OUTPUT_NAME} ${EXTRA_LIBS})

set_target_properties(${lowercase_project_name}_tools_sel PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# 获取所有测试图像
FILE(GLOB TEST_IMAGES ../assets/*.jpg)

# 安装测试图像，安装测试程序到 tools 目录下
install(FILES ${TEST_IMAGES} DESTINATION tools/${lowercase_project_name}_ims)
install (TARGETS ${lowercase_project_name}_tools_sel RUNTIME DESTINATION tools)