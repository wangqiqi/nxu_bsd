# find_depends.cmake
# 此文件负责查找并设置项目所需的依赖库，包括 OpenCV、ONNXRuntime、CUDA 等

# 将所有需要链接的库附加到一个变量中，便于后续子模块使用
set(EXTRA_LIBS "")

# OpenCV 手动配置部分（保留 find_package 并注释）
if(WIN32)
    # find_package(OpenCV REQUIRED)  # 注释掉以使用自定义路径
    set(OpenCV_DIR "E:/build/opencv454-19/install" CACHE PATH "Path to a custom OpenCV build on Windows")
    set(OPENCV_INCLUDE_DIRS "${OpenCV_DIR}/include")
    set(OPENCV_LIB_DIR     "${OpenCV_DIR}/x64/vc16/lib") # 根据实际编译器版本调整

    include_directories(${OPENCV_INCLUDE_DIRS})
    link_directories(${OPENCV_LIB_DIR})

# 如果是 debug 版本，应加载 opencv_world454d.lib
# 否则添加 opencv_world454.lib
    if (BUILD_DEBUG)
        list(APPEND EXTRA_LIBS opencv_world454d.lib) # 根据实际OpenCV库名称调整
    else()
        list(APPEND EXTRA_LIBS opencv_world454.lib) # 根据实际OpenCV库名称调整
    endif()
elseif(NOT WIN32)
    # Linux 下的手动 OpenCV 配置
    # find_package(OpenCV REQUIRED)  # 注释掉以使用自定义路径
    set(OpenCV_DIR "$ENV{HOME}/install/opencv4.5.4" CACHE PATH "Path to a custom OpenCV build on Unix-like systems")
    set(OPENCV_INCLUDE_DIRS "${OpenCV_DIR}/include/opencv4")
    set(OPENCV_LIB_DIR     "${OpenCV_DIR}/lib")

    include_directories(${OPENCV_INCLUDE_DIRS})
    link_directories(${OPENCV_LIB_DIR})

    list(APPEND EXTRA_LIBS opencv_world -fopenmp -lpthread -lm) # 根据实际OpenCV库名称和依赖调整
endif()

# ONNXRuntime 设置部分（假设已安装）
if(WIN32)
    # Windows 下的 ONNXRuntime 配置
    set(ORT_DIR "E:/build/onnxruntime-win-x64-1.15.1") # 根据实际路径调整
    if (USE_GPU)
        set(ORT_DIR "E:/build/onnxruntime-win-x64-gpu-1.15.1") # GPU 版本的路径
    endif()

    set(ORT_INCLUDE_DIR "${ORT_DIR}/include")
    set(ORT_LIB_DIR     "${ORT_DIR}/lib")

    include_directories(${ORT_INCLUDE_DIR})
    link_directories(${ORT_LIB_DIR})

    if (USE_GPU)
        list(APPEND EXTRA_LIBS onnxruntime.lib nvml.lib) # GPU 版本需要链接的库
    else()
        list(APPEND EXTRA_LIBS onnxruntime.lib) # CPU 版本需要链接的库
    endif()
elseif(NOT WIN32)
    # Linux 下的 ONNXRuntime 配置
    set(ORT_DIR "$ENV{HOME}/install/onnxruntime-linux-x64-1.15.1") # 根据实际路径调整
    if (USE_GPU)
        set(ORT_DIR "$ENV{HOME}/install/onnxruntime-linux-x64-gpu-1.15.1") # GPU 版本的路径
    endif()

    set(ORT_INCLUDE_DIR "${ORT_DIR}/include")
    set(ORT_LIB_DIR     "${ORT_DIR}/lib")

    include_directories(${ORT_INCLUDE_DIR})
    link_directories(${ORT_LIB_DIR})

    if (USE_GPU)
        list(APPEND EXTRA_LIBS onnxruntime nvidia-ml) # GPU 版本需要链接的库
    else()
        list(APPEND EXTRA_LIBS onnxruntime) # CPU 版本需要链接的库
    endif()
endif()

# CUDA 路径设置（仅当 USE_GPU 时生效）
if(WIN32 AND USE_GPU)
    set(CUDA_DIR "D:/cuda11-8") # 根据实际路径调整
    set(CUDA_INCLUDE_DIR "${CUDA_DIR}/include")
    set(CUDA_LIB_DIR "${CUDA_DIR}/lib/x64")

    include_directories(${CUDA_INCLUDE_DIR})
    link_directories(${CUDA_LIB_DIR})

    list(APPEND EXTRA_LIBS cudart.lib) # 示例，根据实际情况添加更多CUDA库
endif()

# Linux 下的 CUDA 路径设置（仅当 USE_GPU 时生效）
if(NOT WIN32 AND USE_GPU)
    find_package(CUDA REQUIRED)
    include_directories(${CUDA_INCLUDE_DIRS})

    link_directories(${CUDA_LIB_DIR})
    list(APPEND EXTRA_LIBS ${CUDA_LIBRARIES})
endif()

# 设置检测库及相关依赖函数库的路径
if (WIN32)
    set(NXU_UTILS_DIR "E:/build/nxu_utils")
    set(NXU_UTILS_INCLUDE_DIR "${NXU_UTILS_DIR}/include")
    set(NXU_UTILS_LIB_DIR "${NXU_UTILS_DIR}/lib")

    include_directories(${NXU_UTILS_INCLUDE_DIR})
    link_directories(${NXU_UTILS_LIB_DIR})

    if (USE_GPU)
        list(APPEND EXTRA_LIBS nxu_det_ort_gpu.lib) # GPU 版本需要链接的库
    else()
        list(APPEND EXTRA_LIBS nxu_det_ort_cpu.lib) # CPU 版本需要链接的库
    endif()

    list(APPEND EXTRA_LIBS nxu_utils.lib)
elseif(NOT WIN32)
    set(NXU_UTILS_DIR "$ENV{HOME}/install/nxu_utils")
    set(NXU_UTILS_INCLUDE_DIR "${NXU_UTILS_DIR}/include")
    set(NXU_UTILS_LIB_DIR "${NXU_UTILS_DIR}/lib")

    include_directories(${NXU_UTILS_INCLUDE_DIR})
    link_directories(${NXU_UTILS_LIB_DIR})

    if (USE_GPU)
        list(APPEND EXTRA_LIBS nxu_det_ort_gpu) # GPU 版本需要链接的库
    else()
        list(APPEND EXTRA_LIBS nxu_det_ort_cpu) # CPU 版本需要链接的库
    endif()
    
    list(APPEND EXTRA_LIBS nxu_utils)
endif()

# 输出构建相关信息
message(STATUS ">>>>>>>>>>>>>>>>>>>> message start  >>>>>>>>>>>>>>>>>>>>>>>>>>")
message(STATUS "> Tags: ${${UPPERCASE_PROJECT_NAME}_VERSION}, Commit Hash: ${GIT_COMMIT_MD5}")
message(STATUS "> CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "> Building at ${BUILDDATETIME}")
message(STATUS "> CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "> BUILD_SHARED_LIBRARY = ${BUILD_SHARED_LIBRARY}")
message(STATUS "> OpenCV library status:")
message(STATUS ">   version: ${OpenCV_VERSION}")
message(STATUS ">   include dir: ${OpenCV_INCLUDE_DIRS}")
message(STATUS ">   libraries dir: ${OpenCV_LIB_DIR}")
message(STATUS ">   libraries: ${OpenCV_LIBS}")
message(STATUS "> ONNX Path = ${ORT_DIR}")
message(STATUS ">   include dir: ${ORT_INCLUDE_DIR}")
message(STATUS ">   lib dir: ${ORT_LIB_DIR}")
message(STATUS "> CUDA Path = ${CUDA_DIR}")
message(STATUS ">   include dir: ${CUDA_INCLUDE_DIRS}") # 更正为INCLUDE_DIRS
message(STATUS ">   lib dir: ${CUDA_LIBRARY_DIRS}") # 更正为LIBRARY_DIRS
message(STATUS "> ALL Linked Libs: ${EXTRA_LIBS}")
message(STATUS "<<<<<<<<<<<<<<<<<<<< message end <<<<<<<<<<<<<<<<<<<<<<<<<<<<")


