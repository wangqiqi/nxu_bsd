#include "TargetDetector.hpp"

#include "nxu_common.h"
#include "nxu_bsd_api.h"

#include <fstream>
#include <iostream>
#include <string>

// Include necessary system headers based on the operating system
#if defined(_WIN32)
#include <cstdio>
#include <direct.h>
#include <io.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace std;
int main(int argc, char **argv)
{
    std::cout << " Usage : program_name input_path[output_pos_path][output_neg_path][output_result_path][output_patch_path][bSaveNeg][bSavePatch][bSavePos][bSaveResult][bShowFrame][waitTime] " << endl;

    if (argc < 2)
    {
        cerr << "Usage: program_name input_path [output_pos_path] [output_neg_path] [output_result_path] [output_patch_path] [bSaveNeg] [bSavePatch] [bSavePos] [bSaveResult] [bShowFrame] [waitTime]" << endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_pos_path = "pos";
    std::string output_neg_path = "neg";
    std::string output_result_path = "result";
    std::string output_patch_path = "patch";
    bool bSaveNeg = true;
    bool bSavePatch = true;
    bool bSavePos = true;
    bool bSaveResult = true;
    bool bShowFrame = true;
    int waitTime = 0;

    if (argc >= 3)
        output_pos_path = argv[2];
    if (argc >= 4)
        output_neg_path = argv[3];
    if (argc >= 5)
        output_result_path = argv[4];
    if (argc >= 6)
        output_patch_path = argv[5];
    if (argc >= 7)
        bSaveNeg = static_cast<bool>(atoi(argv[6]));
    if (argc >= 8)
        bSavePatch = static_cast<bool>(atoi(argv[7]));
    if (argc >= 9)
        bSavePos = static_cast<bool>(atoi(argv[8]));
    if (argc >= 10)
        bSaveResult = static_cast<bool>(atoi(argv[9]));
    if (argc >= 11)
        bShowFrame = static_cast<bool>(atoi(argv[10]));
    if (argc >= 12)
        waitTime = atoi(argv[11]);

    // 创建 TargetDetector 实例并配置参数
    TargetDetector detector = TargetDetector();

    // 设置保存选项
    detector.setSaveFlag(bSavePos, bSaveNeg, bSavePatch, bSaveResult, bShowFrame, waitTime);

    // 设置保存路径
    detector.setSavePath(output_pos_path, output_neg_path, output_patch_path, output_result_path);

    // 打印设置的参数信息
    std::cout<<" ------------------------- Flags ----------------------------------" << std::endl;
    std::cout<< "bSaveNeg: " << bSaveNeg << std::endl;
    std::cout<< "bSavePatch: " << bSavePatch << std::endl;
    std::cout<< "bSavePos: " << bSavePos << std::endl;
    std::cout<< "bSaveResult: " << bSaveResult << std::endl;
    std::cout<< "bShowFrame: " << bShowFrame << std::endl;
    std::cout<<" -----------------------------------------------------------" << std::endl;
    
    // 打印保存的路径信息
    std::cout<<" -------------------------- Paths --------------------------" << std::endl;
    std::cout << "output_pos_path: " << output_pos_path << std::endl;
    std::cout << "output_neg_path: " << output_neg_path << std::endl;
    std::cout << "output_patch_path: " << output_patch_path << std::endl;
    std::cout << "output_result_path: " << output_result_path << std::endl;
    std::cout<<" -----------------------------------------------------------" << std::endl;

    // 处理输入资源
    detector.process(input_path);

    return 0;
}