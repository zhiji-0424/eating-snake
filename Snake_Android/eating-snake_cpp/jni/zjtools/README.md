# zjtools

#### 介绍
工具集

#### 软件说明
这是一个 C/C++ 库项目，并且支持所有架构

#### 安装教程
~~1. 请使用 CMake 构建， 暂时不使用 Android.mk~~
~~2. 使用 **ndk-build** 构建，在 *jni/* 下执行 **ndk-build**~~
1. 确保你的项目包含了此目录下的 **Android.mk** 文件。
2. 为你的 **LOCAL_STATIC_LIBRARIES** 加上此模块名：**zjtools** 。
3. 可以根据需要修改 **Android.mk** 或 **代码内容** 。


#### 使用说明
1. 通用库包含 logger, string, utf8转utf16
2. 绘图库包含 draw, shader, android native app glue
3. 过于简单，查看*.h/.hpp*文件即可


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

