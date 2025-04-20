::在主CMakeLists.txt 里设置opencv和ncnn的路径
set build_dir=win10_build

::删除编译目录
rm -rf %build_dir%

::重新创建编译目录
mkdir %build_dir%

::进入编译目录
cd %build_dir%

::配置, 此处可以利用 -D 添加编译选项
cmake -G "Visual Studio 16 2019" -A x64 -DBUILD_SHARED=ON -DBUILD_DEBUG=OFF -DBUILD_TOOLS=ON ..

pause
cd ..
