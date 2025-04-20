#include "nxu_bsd_api.h"
#include "nxu_bsd_gconfig.h"
#include "nxu_bsd_utils.h"

using namespace nxu_bsd_utils;

// 算法句柄结构体
typedef struct _nxu_bsd_handle_s
{
    CNXUBSDUtils *__bsdobj;
} NXU_BSD_Handle_S;

/**
 * @brief 创建算法句柄，加载算法模型，只用创建一次，就可以循环使用，每一次创建对应一次销毁
 *
 * @param  phBSD            算法句柄
 * @return NXU_EXPORTS
 */
NXU_STATUS_CODE_E NXU_BSD_Create(NXU_HANDLE *phBSD)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    // 创建bsd实例
    NXU_BSD_Handle_S *hBSD = new NXU_BSD_Handle_S;
    if (NULL == hBSD)
    {
        return NXU_FAILURE;
    }

    hBSD->__bsdobj = new CNXUBSDUtils();
    if (NULL == hBSD->__bsdobj)
    {
        return NXU_FAILURE;
    }

    // 返回句柄
    *phBSD = (NXU_HANDLE)hBSD;

    return s32Ret;
}


NXU_STATUS_CODE_E NXU_BSD_CreateWithModel(NXU_HANDLE* phBSD,
    char* modelPath)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    // 创建bsd实例
    NXU_BSD_Handle_S* hBSD = new NXU_BSD_Handle_S;
    if (NULL == hBSD)
    {
        return NXU_FAILURE;
    }

    hBSD->__bsdobj = new CNXUBSDUtils(modelPath);
    if (NULL == hBSD->__bsdobj)
    {
        return NXU_FAILURE;
    }

    // 返回句柄
    *phBSD = (NXU_HANDLE)hBSD;

    return s32Ret;
}


/**
 * @brief 销毁算法句柄，使用完后记得销毁算法句柄
 *
 * @param  hBSD             算法句柄
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_BSD_Destroy(NXU_HANDLE hBSD)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;
    NXU_BSD_Handle_S *pHandle = (NXU_BSD_Handle_S *)hBSD;
    if (NULL == pHandle)
    {
        return NXU_FAILURE;
    }

    if (NULL == pHandle->__bsdobj)
    {
        return NXU_FAILURE;
    }
    else
    {
        delete pHandle->__bsdobj;
        pHandle->__bsdobj = NULL;
    }

    delete pHandle;
    pHandle = NULL;

    return s32Ret;
}

/**
 * @brief 获取算法的版本号
 *
 * @param  pcLibVerStr      算法库版本号
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_BSD_GetLibVer(NXU_S8 *pcLibVerStr)
{

    // 获取版本号
    std::cout << "\n=============== LIB INFO =====================\n";
    std::cout << "PROJECT_NAME: " << BSD_PROJECT_NAME << std::endl;
    std::cout << "LIB_VERSION: " << BSD_VERSION << std::endl;
    std::cout << "LIB_COMMIT_MD5: " << BSD_COMMIT << std::endl;
    std::cout << "BUILD_DATETIME: " << BSD_DATETIME << std::endl;
    std::cout<< "==============================================\n";

    sprintf((char*) pcLibVerStr, "%s-%s-%s-%s", BSD_PROJECT_NAME, 
            BSD_VERSION, BSD_COMMIT, BSD_DATETIME);
            
    return NXU_SUCCESS;
}

/**
 * @brief 算法处理
 *
 * @param  hBSD             算法句柄
 * @param  pstImage         图像数据
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_BSD_Process(NXU_HANDLE hBSD, const NXU_IMAGE_S *pstImage)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    NXU_BSD_Handle_S *pHandle = (NXU_BSD_Handle_S *)hBSD;
    if (NULL == pHandle || NULL == pstImage || NULL == pstImage->pData)
    {
        return NXU_FAILURE;
    }

#ifdef SHOW_DEBUG_TIME
    double dtStart, dtEnd;
    dtStart = __get_current_time_proc();
#endif // SHOW_DEBUG_TIME

    // 处理图像
    s32Ret = (NXU_STATUS_CODE_E)(pHandle->__bsdobj->process(pstImage));
    if (s32Ret != NXU_SUCCESS)
    {
        return s32Ret;
    }

#ifdef SHOW_DEBUG_TIME
    dtEnd = __get_current_time_proc();
    std::cout << "++++++  In BSD API: Process Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
    dtStart = __get_current_time_proc();
#endif // SHOW_DEBUG_TIME

    return s32Ret;
}

/**
 * @brief 获取结果
 *
 * @param  hBSD             算法句柄
 * @param  pstBSDResult     BSD结果
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_BSD_GetResults(NXU_HANDLE hBSD, NXU_BSD_Result_S *pstBSDResult)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;
    NXU_BSD_Handle_S *pHandle = (NXU_BSD_Handle_S *)hBSD;
    if (NULL == pHandle || NULL == pstBSDResult)
    {
        return NXU_FAILURE;
    }

    // 获取结果，通过参数返回出来
    s32Ret = (NXU_STATUS_CODE_E)(pHandle->__bsdobj->get_results(pstBSDResult));

    return s32Ret;
}
