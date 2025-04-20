#!/bin/bash

# 目录名
build_dir=ubuntu_build

# 删除编译目录
rm -rf ${build_dir}

# 重新创建编译目录
mkdir ${build_dir}

# 进入编译目录
pushd ${build_dir}

# 配置, 此处可以利用 -D 添加编译选项
# 如果要用 Release 版本，请设置 BUILD_DEBUG=OFF
cmake -DBUILD_SHARED=ON -DBUILD_DEBUG=OFF  -DBUILD_TOOLS=ON  -DUSE_GPU=OFF ..

# 编译
make -j8

# 安装
make install 

# 退出目录
popd
