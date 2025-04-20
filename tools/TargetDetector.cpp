#include "TargetDetector.hpp"

TargetDetector::TargetDetector()
{
    m_frmNum = 0;

    m_bSaveNeg = true;
    m_bSavePatch = true;
    m_bSavePos = true;
    m_bSaveResult = true;
    m_bShow = false;
    m_waitTime = 1;

    m_patch_path = "patch";
    m_result_path = "result";
    m_pos_path = "pos";
    m_neg_path = "neg";

    // 初始化目标检测句柄
    NXU_HANDLE hBSD;

    int ret = NXU_BSD_Create(&hBSD);
    // int ret = NXU_BSD_CreateWithModel(&hBSD, "bsd.weights");
    if (ret == NXU_SUCCESS)
    {
        hProcessID = hBSD;
    }
    else
    {
        throw std::runtime_error("Failed to initialize BSD handler");
    }
}

TargetDetector::~TargetDetector()
{
    // 释放目标检测句柄资源
    if (hProcessID != nullptr)
    {
        // 假设 nxu_release_bsd 是用于释放目标检测句柄的函数
        NXU_BSD_Destroy(hProcessID);
        hProcessID = nullptr;
    }
}

bool TargetDetector::setSaveFlag(bool bSavePos, bool bSaveNeg, bool bSavePatch,
                                 bool bSaveResult, bool bShow, int waitTime)
{
    m_bSavePos = bSavePos;
    m_bSaveNeg = bSaveNeg;
    m_bSavePatch = bSavePatch;
    m_bSaveResult = bSaveResult;
    m_bShow = bShow;
    m_waitTime = waitTime;

    return true;
}

bool TargetDetector::setSavePath(std::string strPosPath, std::string strNegPath,
                                 std::string strPatchPath, std::string strResultPath)
{
    m_pos_path = strPosPath;
    m_neg_path = strNegPath;
    m_patch_path = strPatchPath;
    m_result_path = strResultPath;

    if (!m_outManager.directory_exists(m_pos_path))
    {
        m_outManager.create_directories(m_pos_path);
    }
    if (!m_outManager.directory_exists(m_neg_path))
    {
        m_outManager.create_directories(m_neg_path);
    }
    if (!m_outManager.directory_exists(m_patch_path))
    {
        m_outManager.create_directories(m_patch_path);
    }
    if (!m_outManager.directory_exists(m_result_path))
    {
        m_outManager.create_directories(m_result_path);
    }

    return true;
}

bool TargetDetector::process(const std::string &input_path)
{
    // 根据资源类型进行不同的处理
    m_resource_type = m_outManager.detect_resource_type(input_path);

    bool bProcessFlag = true;

    std::cout << "path: " << input_path << std::endl;

    switch (m_resource_type)
    {
    case ResourceType::Image:
        bProcessFlag = process_image(input_path);
        break;
    case ResourceType::Directory:
        bProcessFlag = process_path(input_path);
        break;
    case ResourceType::Video:
        bProcessFlag = process_video(input_path);
        break;
    case ResourceType::RTSP:
        bProcessFlag = process_rtsp(input_path);
        break;
    default:
        std::cerr << "Error: 未知的资源类型。\n";
        return false;
    }
    return bProcessFlag;
}

bool TargetDetector::process_image(const std::string &input_path)
{
    std::cout << "processing image " << input_path << " ... " << std::endl;

    // 加载图像
    cv::Mat input_img = cv::imread(input_path);
    if (input_img.empty())
    {
        std::cerr << "Error: 输入图像为空。\n";
        return false;
    }

    NXU_S32 s32W = input_img.cols, s32H = input_img.rows;
    NXU_IMAGE_S stImg;
    stImg.s32W = s32W;
    stImg.s32H = s32H;
    stImg.eFormat = NXU_IMAGE_FORMAT_BGR_PACKED;
    stImg.pData = input_img.data;

    // 处理图像
    int ret = NXU_BSD_Process(hProcessID, &stImg);
    if (ret != NXU_SUCCESS)
    {
        return false;
    }

    // 获取结果并遍历目标
    NXU_BSD_Result_S stResults;
    ret = NXU_BSD_GetResults(hProcessID, &stResults);
    if (ret != NXU_SUCCESS)
    {
        return false;
    }

    cv::Mat result_frame = input_img.clone();
    for (int i = 0; i < stResults.s32TgtNum; i++)
    {
        NXU_BSD_TgtItem_S stItem = stResults.astTgtItems[i];
        NXU_RECT_S stRect = stItem.stRect;
        NXU_S32 s32X1 = stRect.stTopLeft.s32X,
                s32Y1 = stRect.stTopLeft.s32Y,
                s32X2 = stRect.stBottomRight.s32X,
                s32Y2 = stRect.stBottomRight.s32Y;

        if (s32X1 < 0)
            s32X1 = 0;
        if (s32Y1 < 0)
            s32Y1 = 0;
        if (s32X2 >= s32W)
            s32X2 = s32W - 1;
        if (s32Y2 >= s32H)
            s32Y2 = s32H - 1;

        // 输出类别，概率和坐标
        std::cout << stItem.enTgtType << " " << stItem.f32Prob << " "
                  << stItem.stRect.stTopLeft.s32X << " " << stItem.stRect.stTopLeft.s32Y << " "
                  << stItem.stRect.stBottomRight.s32X << " " << stItem.stRect.stBottomRight.s32Y << std::endl;

        // 将标签和概率写到框子上
        char text[256];
        int labelID = (int)(stItem.enTgtType);
        // 根据标签的大小确定颜色
        cv::Scalar color = colors[labelID];

        sprintf(text, "%d--%.2f%%", labelID, stItem.f32Prob * 100.f);
        cv::putText(result_frame, text, cv::Point(s32X1, s32Y1 + 12), cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
        cv::Rect rect(s32X1, s32Y1, s32X2 - s32X1, s32Y2 - s32Y1);
        cv::rectangle(result_frame, rect, color, 2);

        cv::Mat target_patch = input_img(rect);

        if (m_bSavePatch)
        {
            std::string patch_file_name = m_outManager.extract_filename(input_path) + "_" + std::to_string(i) + ".jpg";
            save_img(m_patch_path, patch_file_name, target_patch);
        }
    }

    if (stResults.s32TgtNum > 0)
    {
        if (m_bSavePos)
        {
            std::string pos_file_name = m_outManager.extract_filename(input_path) + ".jpg";
            save_img(m_pos_path, pos_file_name, input_img);
        }

        if (m_bSaveResult)
        {
            std::string result_file_name = m_outManager.extract_filename(input_path) + ".jpg";
            save_img(m_result_path, result_file_name, result_frame);
        }
    }
    else
    {
        if (m_bSaveNeg)
        {
            std::string neg_file_name = m_outManager.extract_filename(input_path) + ".jpg";
            save_img(m_neg_path, neg_file_name, input_img);
        }
    }

    if (m_bShow)
    {
        cv::imshow("Result", result_frame);
        cv::waitKey(m_waitTime);
    }

    return true;
}

bool TargetDetector::process_path(const std::string &input_path)
{
    // 获取所有图像文件
    std::vector<std::string> image_files = m_outManager.get_all_image_files_in_path(input_path);

    std::cout << "images num: " << image_files.size() << std::endl;

    bool flag = true;

    // 遍历图像文件
    for (const auto &image_file : image_files)
    {
        flag &= process_image(image_file); // 使用逻辑与操作符合并结果
    }

    return flag; // 返回最终处理结果
}

bool TargetDetector::process_video(const std::string &input_path)
{
    // 读取视频
    cv::VideoCapture cap(input_path);

    m_frmNum = 0;

    std::string video_str = m_outManager.extract_filename(input_path);

    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频: " << input_path << std::endl;
        return false;
    }

    // 读取帧
    cv::Mat input_img;
    cap >> input_img;

    if (input_img.empty())
    {
        std::cerr << "无法读取视频帧" << std::endl;
        return false;
    }

    NXU_S32 s32W = input_img.cols, s32H = input_img.rows;
    NXU_IMAGE_S stImg;
    stImg.s32W = s32W;
    stImg.s32H = s32H;
    stImg.eFormat = NXU_IMAGE_FORMAT_BGR_PACKED;

    while (true)
    {
        cap >> input_img;
        if (input_img.empty())
        {
            std::cerr << "无法读取视频帧" << std::endl;
            break;
        }

        stImg.pData = input_img.data;

        m_frmNum++;

        // 处理图像
        int ret = NXU_BSD_Process(hProcessID, &stImg);
        if (ret != NXU_SUCCESS)
        {
            return false;
        }

        // 获取结果并遍历目标
        NXU_BSD_Result_S stResults;
        ret = NXU_BSD_GetResults(hProcessID, &stResults);
        if (ret != NXU_SUCCESS)
        {
            return false;
        }

        video_str = video_str + "_" + std::to_string(m_frmNum);

        cv::Mat result_frame = input_img.clone();
        for (int i = 0; i < stResults.s32TgtNum; i++)
        {
            NXU_BSD_TgtItem_S stItem = stResults.astTgtItems[i];
            NXU_RECT_S stRect = stItem.stRect;
            NXU_S32 s32X1 = stRect.stTopLeft.s32X,
                    s32Y1 = stRect.stTopLeft.s32Y,
                    s32X2 = stRect.stBottomRight.s32X,
                    s32Y2 = stRect.stBottomRight.s32Y;

            if (s32X1 < 0)
                s32X1 = 0;
            if (s32Y1 < 0)
                s32Y1 = 0;
            if (s32X2 >= s32W)
                s32X2 = s32W - 1;
            if (s32Y2 >= s32H)
                s32Y2 = s32H - 1;

            cv::Rect rect(s32X1, s32Y1, s32X2 - s32X1, s32Y2 - s32Y1);
            cv::rectangle(result_frame, rect, cv::Scalar(0, 255, 0), 2);

            cv::Mat target_patch = input_img(rect);

            if (m_bSavePatch)
            {
                std::string patch_file_name = video_str + "_" + std::to_string(i) + ".jpg";
                save_img(m_patch_path, patch_file_name, target_patch);
            }
        }

        if (m_bSaveResult)
        {
            std::string result_file_name = video_str + ".jpg";
            save_img(m_result_path, result_file_name, result_frame);
        }

        if (stResults.s32TgtNum > 0)
        {
            if (m_bSavePos)
            {
                std::string pos_file_name = video_str + ".jpg";
                save_img(m_pos_path, pos_file_name, input_img);
            }
        }
        else
        {
            if (m_bSaveNeg)
            {
                std::string neg_file_name = video_str + ".jpg";
                save_img(m_neg_path, neg_file_name, input_img);
            }
        }
    }

    return true;
}

bool TargetDetector::process_rtsp(const std::string &input_path)
{
    return process_video(input_path);
}

bool TargetDetector::save_img(const std::string save_dir, const std::string save_name,
                              cv::Mat &img)
{

    if (!m_outManager.directory_exists(save_dir))
    {
        m_outManager.create_directories(save_dir);
    }

    if (img.empty())
    {
        std::cout << "save_img: img is empty" << std::endl;
        return false;
    }

    if (m_outManager.is_image(save_name))
    {
        cv::imwrite(save_dir + PATH_SEPARATOR_CHAR + save_name, img);
    }
    else
    {
        std::cout << "save_img: save_name is not image" << std::endl;
        return false;
    }

    return true;
}