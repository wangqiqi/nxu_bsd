#ifndef __NXU_BSD_UTILS_H__
#define __NXU_BSD_UTILS_H__

#include "nxu_det_yolov8_ort.h"
#include "nxu_bsd_api.h"
#include "nxu_bsd_gconfig.h"
#include "nxu_utils.h"

using namespace nxu_det_yolov8_ort;
using namespace nxu_utils;

namespace nxu_bsd_utils
{

    class CNXUBSDUtils
    {
    public:
        /* 构造函数 */
        CNXUBSDUtils();

        /* 用路径构造初始化模型 */
        CNXUBSDUtils(const std::string &modelPath);

        // 处理
        NXU_STATUS_CODE_E process(const NXU_IMAGE_S *pstImage);

        // 获得结果
        NXU_STATUS_CODE_E get_results(NXU_BSD_Result_S *pstResult);

        // 删除目标检测器
        ~CNXUBSDUtils();

    private:
        /* 清理目标信息 */
        void __clean_objs();

        /* 规则后处理 */
        NXU_STATUS_CODE_E __post_process();

        // 将图像转到指定大小
        void __scaleIm(cv::Mat &src, cv::Mat &dst, int s32DesignedWH, float *scale);

    private:
        /* 检测返回的结果 */
        NXU_BSD_Result_S m_stResults;

        // 检测器
        CYOLOV8_ORT_Detector *m_PCYOLOV8DetNet;

        /* 检测结果 */
        std::vector<NXU_DetTarget_S> m_vecObjs;

        // 原始图像
        cv::Mat m_bgrIn;

        // 缩放的图像
        cv::Mat m_TgtSzbgrIn;

        // 缩放比例
        float m_fScale;
    };

}

#endif // !__NXU_BSD_UTILS_H__
