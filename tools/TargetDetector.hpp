#ifndef __TARGET_DETECTOR_HPP__
#define __TARGET_DETECTOR_HPP__

// 包含必要的头文件
#include "OutputFileManager.hpp"
#include "nxu_bsd_api.h"
#include "nxu_common.h"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

// 定义颜色数组，方便后续根据不同标签确定不同颜色
static const cv::Scalar colors[] = {
    cv::Scalar(255, 0, 0),
    cv::Scalar(0, 255, 0),
    cv::Scalar(0, 0, 255),
    cv::Scalar(255, 255, 0),
    cv::Scalar(255, 0, 255),
    cv::Scalar(0, 255, 255),
    cv::Scalar(255, 127, 0),
    cv::Scalar(255, 0, 127),
    cv::Scalar(0, 255, 127),
    cv::Scalar(127, 0, 255),
    cv::Scalar(127, 255, 0),
    cv::Scalar(0, 127, 255),
    cv::Scalar(127, 0, 127),
    cv::Scalar(0, 127, 127),
    cv::Scalar(127, 127, 0),
    cv::Scalar(127, 127, 127)};

/**
 * @class 目标检测器
 *
 * @brief 本类负责对各类输入资源（如图像、视频、RTSP流或包含图像的目录）进行目标检测。
 *
 * `TargetDetector` 类提供了初始化目标检测引擎、处理输入数据、保存结果以及
 * 处理不同类型的输入路径的方法。它还维护一个帧号计数器、显示标志、目标检测句柄、文件系统工具对象。
 */
class TargetDetector
{
private:
    // 帧号
    int m_frmNum;

    // 保存选项
    bool m_bSavePatch;  // 是否保存 patch
    bool m_bSavePos;    // 是否保存 pos
    bool m_bSaveNeg;    // 是否保存 neg
    bool m_bSaveResult; // 是否保存结果
    bool m_bShow;       // 是否显示
    int m_waitTime;     // waittime

    // 模型路径
    std::string m_model_path;

    // 保存 patch 的路径
    std::string m_patch_path;

    // 保存 pos 的路径
    std::string m_pos_path;

    // 保存 neg 的路径
    std::string m_neg_path;

    // 保存结果的路径
    std::string m_result_path;

    // 目标检测句柄
    NXU_HANDLE hProcessID;

    // 路径管理器
    OutputFileManager m_outManager;

    // 资源类型
    ResourceType m_resource_type;

public:
    /**
     * @brief 构造 `TargetDetector` 实例，初始化目标检测引擎。
     *
     * 该构造函数通过调用适当的初始化函数来设置目标检测引擎，并读取  图像。
     * 它还会打印启动消息。
     *
     * @throws std::runtime_error 如果 BSD 处理器的初始化失败。
     */
    TargetDetector();

    /**
     * @brief 销毁 `TargetDetector` 实例，释放目标检测句柄。
     *
     * 该析构函数确保在实例销毁前与目标检测句柄关联的所有资源都被正确释放。
     */
    ~TargetDetector();

    /**
     * @brief 配置是否保存结果以及相关路径。
     *
     * @param bSavePos 是否保存正例结果。
     * @param bSaveNeg 是否保存负例结果。
     * @param bSavePatch 是否保存补丁。
     * @param bSaveResult 是否保存最终检测结果。
     * @param bShow 是否在窗口中显示检测结果。
     * @param waitTime 在显示结果时等待的键盘事件时间（毫秒）。
     * @return 配置是否成功。
     */
    bool setSaveFlag(bool bSavePos, bool bSaveNeg, bool bSavePatch, bool bSaveResult,
                     bool bShow, int waitTime);

    /**
     * @brief 配置各个结果的保存路径。
     *
     * @param strPosPath 正例结果的保存路径。
     * @param strNegPath 负例结果的保存路径。
     * @param strPatchPath 补丁的保存路径。
     * @param strResultPath 最终检测结果的保存路径。
     * @return 配置是否成功。
     */
    bool setSavePath(std::string strPosPath, std::string strNegPath,
                     std::string strPatchPath, std::string strResultPath);

    /**
     * @brief 对给定的输入路径进行目标检测处理。
     *
     * @param input_path 输入资源的路径。
     * @return 处理是否成功。
     */
    bool process(const std::string &input_path);

private:
    // 对图像进行处理
    bool process_image(const std::string &input_path);

    // 对路径进行处理
    bool process_path(const std::string &input_path);

    // 对视频进行处理
    bool process_video(const std::string &input_path);

    // 对 RTSP 流进行处理
    bool process_rtsp(const std::string &input_path);

    // 保存图片
    bool save_img(const std::string save_dir, const std::string save_name,
                  cv::Mat &img);
};

#endif /* __TARGET_DETECTOR_HPP__ */