#include "OutputFileManager.hpp"

/**
 * @brief OutputFileManager 构造函数，传入基础输出目录
 *
 * @param[in] base_output_dir 基础输出目录路径
 */
OutputFileManager::OutputFileManager(const std::string &base_output_dir)
    : base_dir_(base_output_dir)
{
}

// /**
//  * @brief 根据输入路径和自定义名称创建目录，并返回完整路径
//  *
//  * @param[in] directory_path 输入目录路径
//  * @param[in] custom_name 自定义目录名称（可选，默认为空）
//  *
//  * @return 创建的完整目录路径
//  */
// std::string OutputFileManager::create_directory_from_path(const std::string &directory_path, const std::string &custom_name) const
// {
//     std::string full_path = combine_paths(directory_path, custom_name);
//     create_directories(full_path);
//     return full_path;
// }

// 创建指定路径的目录
void OutputFileManager::create_directories(const std::string &path)
{
// 使用已有的跨平台代码创建目录
#ifdef _WIN32
    int result = _mkdir(path.c_str());
#elif defined(__unix__) || defined(__APPLE__)
    mode_t permissions = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    int result = mkdir(path.c_str(), permissions);
#else
    // 简化示例，此处表示不支持的平台无法创建目录
    return;
#endif

    // 检查并处理错误
    if (result != 0 && errno != EEXIST)
    {
        throw std::runtime_error("Failed to create directory: " + path);
    }
}

/**
 * @brief 检测给定路径所代表的资源类型
 *
 * @param[in] path 输入路径
 *
 * @return 资源类型（如图像、视频、目录、RTSP流或未知）
 */
ResourceType OutputFileManager::detect_resource_type(const std::string &path) const
{
    if (is_directory(path))
    {
        return ResourceType::Directory;
    }

    if (is_rtsp_stream(path))
    {
        return ResourceType::RTSP;
    }

    if (is_image(path))
    {
        return ResourceType::Image;
    }
    else if (is_video(path))
    {
        return ResourceType::Video;
    }

    return ResourceType::Unknown;
}

/**
 * @brief 获取指定目录下的所有图片文件
 *
 * @param[in] directory_path 目录路径
 *
 * @return 包含所有图片文件路径的字符串向量
 */
std::vector<std::string> OutputFileManager::get_all_image_files_in_path(const std::string &directory_path) const
{
    std::vector<std::string> image_files;
    find_images(directory_path, image_files);
    return image_files;
}

/**
 * @brief 检查路径是否为现有目录
 *
 * @param[in] path 待检查的路径
 *
 * @return 若路径为现有目录，返回 true；否则返回 false
 */
bool OutputFileManager::directory_exists(const std::string &path) const
{
// 使用已有的跨平台代码检查目录是否存在
#ifdef _WIN32
    struct _stat info;
    return (_stat(path.c_str(), &info) == 0 && (info.st_mode & _S_IFDIR) != 0);
#elif defined(__unix__) || defined(__APPLE__)
    struct stat info;
    return (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode));
#else
    // 实际项目中需要适配所有目标平台
    return false; // 简化示例，此处表示不支持的平台无法检查目录是否存在
#endif
}

/**
 * @brief 提取文件名（不含路径）
 *
 * @param[in] path 包含文件名的完整路径
 *
 * @return 文件名（不含路径）
 */
std::string OutputFileManager::extract_filename(const std::string &path) const
{
    auto pos = path.find_last_of(PATH_SEPARATOR_CHAR);
    return pos == std::string::npos ? path : path.substr(pos + 1);
}

/**
 * @brief 获取文件扩展名
 *
 * @param[in] input_path 输入文件路径
 *
 * @return 文件扩展名（不含点）
 */
std::string OutputFileManager::determine_file_extension(const std::string &input_path) const
{
    auto pos = input_path.find_last_of('.');
    return pos == std::string::npos ? "" : input_path.substr(pos);
}

/**
 * @brief 判断给定路径是否为目录
 *
 * @param[in] path 待判断的路径
 *
 * @return 若路径为目录，返回 true；否则返回 false
 */
bool OutputFileManager::is_directory(const std::string &path) const
{
    return directory_exists(path);
}

/**
 * @brief 判断字符串是否为RTSP流地址
 *
 * @param[in] input_path 待判断的字符串
 *
 * @return 若输入路径为RTSP流地址，返回 true；否则返回 false
 */
bool OutputFileManager::is_rtsp_stream(const std::string &input_path) const
{
    return input_path.find("rtsp://", 0) == 0;
}

/**
 * @brief 判断文件名是否指向图片类型的文件
 *
 * @param[in] file_name 待判断的文件名，可能携带路径
 *
 * @return 若文件名指向图片类型的文件，返回 true；否则返回 false
 */
bool OutputFileManager::is_image(const std::string &file_name) const
{
    // 查找文件名中最后一个 '.' 的位置
    size_t ext_pos = file_name.rfind('.');
    if (ext_pos == std::string::npos)
    {
        // 没有找到 '.'，不是有效的带扩展名文件名，返回 false
        return false;
    }

    // 提取扩展名（不含前导点）
    std::string ext = file_name.substr(ext_pos);

    // std::cout<<"ext::::::::" << ext << std::endl;

    // 使用现有的图片扩展名集合判断
    static const std::vector<std::string> image_extensions = {".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff" /* 其他图像扩展名 */};

    for (const auto &img_ext : image_extensions)
    {
        if (ext == img_ext)
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief 判断文件名是否指向视频类型的文件
 *
 * @param[in] file_name 待判断的文件名，可能携带路径
 *
 * @return 若文件名指向视频类型的文件，返回 true；否则返回 false
 */
bool OutputFileManager::is_video(const std::string &file_name) const
{
    // 查找文件名中最后一个 '.' 的位置
    size_t ext_pos = file_name.rfind('.');
    if (ext_pos == std::string::npos)
    {
        // 没有找到 '.'，不是有效的带扩展名文件名，返回 false
        return false;
    }

    // 提取扩展名（不含前导点）
    std::string ext = file_name.substr(ext_pos);

    // 使用现有的视频扩展名集合判断
    static const std::vector<std::string> video_extensions = {".mp4", ".avi", ".mov", ".wmv", ".mkv", ".flv" /* 其他视频扩展名 */};

    for (const auto &video_ext : video_extensions)
    {
        if (ext == video_ext)
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief 辅助方法：合并路径与子路径，确保正确添加分隔符
 *
 * @param[in] dir_path 父目录路径
 * @param[in] sub_path 子路径或文件名
 *
 * @return 合并后的完整路径
 */
std::string OutputFileManager::combine_paths(const std::string &dir_path, const std::string &sub_path) const
{
    std::string full_path = dir_path;
    if (!sub_path.empty() && full_path.back() != PATH_SEPARATOR_CHAR)
    {
        full_path += PATH_SEPARATOR_CHAR;
    }
    full_path += sub_path;
    return full_path;
}

/**
 * @brief 在指定目录下查找所有图片文件
 *
 * @param[in] directory_path 目录路径
 * @param[out] image_files 找到的图片文件路径列表
 */
void OutputFileManager::find_images(const std::string &directory_path, std::vector<std::string> &image_files) const
{
#ifdef _WIN32
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile((directory_path + "\\*.jpg").c_str(), &data); // 示例只查找.jpg，可扩展其他格式
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string full_path = directory_path + "\\" + data.cFileName;
                image_files.push_back(full_path);
            }
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
#else
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory_path.c_str())) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            std::string filename(entry->d_name);
            // 检查是否为图片文件（假设.jpg .jpeg .png等）
            if (is_image(filename))
            {
                std::string full_path = directory_path + PATH_SEPARATOR_CHAR + filename;
                image_files.push_back(full_path);
            }
        }
        closedir(dir);
    }
    else
    {
        throw std::runtime_error("Failed to open directory: " + directory_path);
    }
#endif
}