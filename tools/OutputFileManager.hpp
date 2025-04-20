#ifndef __OUTPUT_FILEMANAGER_HPP__
#define __OUTPUT_FILEMANAGER_HPP__

#include <algorithm> // 用于std::find_if和std::end
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

// 为了简化演示，仅提供Windows与POSIX兼容系统的简单封装
#if defined(_WIN32)
#include <Windows.h> // Windows环境下用于创建目录
#include <direct.h>  // Windows环境下用于创建目录
#elif defined(__unix__) || defined(__APPLE__)
#include <dirent.h>   // POSIX环境下用于遍历目录
#include <sys/stat.h> // POSIX环境下用于创建目录
#else
#error "Unsupported platform!"
#endif

// 或者针对Windows和POSIX环境分别定义
#ifdef _WIN32
const char PATH_SEPARATOR_CHAR = '\\';
#else
const char PATH_SEPARATOR_CHAR = '/';
#endif

/**
 * @brief 资源类型枚举
 */
enum class ResourceType
{
    Image,
    Video,
    Directory,
    RTSP,
    Unknown
};

/**
 * @class OutputFileManager
 *
 * @brief 文件系统助手类，包含创建目录、检测资源类型等功能
 *
 * `OutputFileManager` 类提供了一系列与文件系统交互的辅助方法，如创建目录、检测输入路径所代表的资源类型、获取指定目录下的所有图片文件等。此类旨在简化文件系统操作，提高代码的可移植性。
 */
class OutputFileManager
{
public:
    /**
     * @brief 默认构造函数
     */
    OutputFileManager() = default;

    /**
     * @brief 构造函数，传入基础输出目录
     *
     * @param[in] base_output_dir 基础输出目录路径
     */
    explicit OutputFileManager(const std::string &base_output_dir);

    // /**
    //  * @brief 根据输入路径和自定义名称创建目录，并返回完整路径
    //  *
    //  * @param[in] directory_path 输入目录路径
    //  * @param[in] custom_name 自定义目录名称（可选，默认为空）
    //  *
    //  * @return 创建的完整目录路径
    //  */
    // std::string create_directory_from_path(const std::string &directory_path, const std::string &custom_name = "") const;

    /**
     * @brief 检测给定路径所代表的资源类型
     *
     * @param[in] path 输入路径
     *
     * @return 资源类型（如图像、视频、目录、RTSP流或未知）
     */
    ResourceType detect_resource_type(const std::string &path) const;

    /**
     * @brief 获取指定目录下的所有图片文件
     *
     * @param[in] directory_path 目录路径
     *
     * @return 包含所有图片文件路径的字符串向量
     */
    std::vector<std::string> get_all_image_files_in_path(const std::string &directory_path) const;

    /**
     * @brief 创建指定路径的目录
     *
     * @param[in] path 待创建目录的路径
     */
    void create_directories(const std::string &path);

    /**
     * @brief 检查路径是否为现有目录
     *
     * @param[in] path 待检查的路径
     *
     * @return 若路径为现有目录，返回 true；否则返回 false
     */
    bool directory_exists(const std::string &path) const;

    /**
     * @brief 提取文件名（不含路径）
     *
     * @param[in] path 包含文件名的完整路径
     *
     * @return 文件名（不含路径）
     */
    std::string extract_filename(const std::string &path) const;

    /**
     * @brief 获取文件扩展名
     *
     * @param[in] input_path 输入文件路径
     *
     * @return 文件扩展名（不含点）
     */
    std::string determine_file_extension(const std::string &input_path) const;

    /**
     * @brief 判断给定路径是否为目录
     *
     * @param[in] path 待判断的路径
     *
     * @return 若路径为目录，返回 true；否则返回 false
     */
    bool is_directory(const std::string &path) const;

    /**
     * @brief 判断字符串是否为RTSP流地址
     *
     * @param[in] input_path 待判断的字符串
     *
     * @return 若输入路径为RTSP流地址，返回 true；否则返回 false
     */
    bool is_rtsp_stream(const std::string &input_path) const;

    /**
     * @brief 判断文件名是否指向图片类型的文件
     *
     * @param[in] file_name 待判断的文件名，可能携带路径
     *
     * @return 若文件名指向图片类型的文件，返回 true；否则返回 false
     */
    bool is_image(const std::string &ext) const;

    /**
     * @brief 判断文件名是否指向视频类型的文件
     *
     * @param[in] file_name 待判断的文件名，可能携带路径
     *
     * @return 若文件名指向视频类型的文件，返回 true；否则返回 false
     */
    bool is_video(const std::string &ext) const;

    /**
     * @brief 辅助方法：合并路径与子路径，确保正确添加分隔符
     *
     * @param[in] dir_path 父目录路径
     * @param[in] sub_path 子路径或文件名
     *
     * @return 合并后的完整路径
     */
    std::string combine_paths(const std::string &dir_path, const std::string &sub_path) const;

    /**
     * @brief 在指定目录下查找所有图片文件
     *
     * @param[in] directory_path 目录路径
     * @param[out] image_files 找到的图片文件路径列表
     */
    void find_images(const std::string &directory_path, std::vector<std::string> &image_files) const;

    std::string base_dir_; // 基础输出目录
};
#endif /* __OUTPUT_FILEMANAGER_HPP__ */