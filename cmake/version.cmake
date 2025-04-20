# 获取Git提交MD5值（用于版本信息）
execute_process(
    COMMAND git describe --always
    TIMEOUT 5 # 设置超时时间，如果获取版本描述耗时超过5秒，则超时
    RESULT_VARIABLE GIT_DESCRIBE_RESULT
    OUTPUT_VARIABLE GIT_COMMIT_MD5
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 检查git describe命令执行结果
if(NOT GIT_DESCRIBE_RESULT EQUAL 0)
    message(WARNING "Failed to retrieve Git commit MD5. Falling back to default versioning.")
    set(GIT_COMMIT_MD5 "unknown-commit")
endif()

# 获取当前时间和日期，并设置项目版本号
string(TIMESTAMP BUILDDATETIME "%Y-%m-%d %H:%M:%S") # 当前构建时间戳
string(TIMESTAMP BUILDDATE     "%Y%m%d")              # 当前日期

# 将设置的项目名转为全大写或者全小写，一般情况下，全小写用于文件名指代，全大写为库名指代
# 例如：将"MyProject"设置为项目名，则全大写为"MYPROJECT"，全小写为"myproject"
string(TOLOWER ${PROJECT_NAME} lowercase_project_name)
string(TOUPPER ${PROJECT_NAME} UPPERCASE_PROJECT_NAME)

# 设置项目版本号
set(PROJECT_MAJOR_VERSION 2)
set(PROJECT_MINOR_VERSION 0)
set(PROJECT_PATCH_VERSION ${BUILDDATE}) # patch版本号为日期
set(PROJECT_VERSION       "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")
set(WEARE "@2024 NXU Co,.Ltd." CACHE STRING "公司信息")

message(STATUS "Building from git tag ${GIT_COMMIT_MD5}")
add_definitions(-DBUILD_TAG_VERSION="${GIT_COMMIT_MD5}") # 添加预处理器定义以供代码使用版本号

set(PROJECT_SIGNATURE_STR 
    "${WEARE}, || Project: ${CMAKE_PROJECT_NAME} || Version: ${PROJECT_VERSION} || DateTime: ${BUILDDATETIME} || MD5:${GIT_COMMIT_MD5}"
    CACHE STRING "项目签名信息字符串")