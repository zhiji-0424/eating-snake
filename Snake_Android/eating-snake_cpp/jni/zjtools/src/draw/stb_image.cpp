#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 将顶上的宏(STB_IMAGE_IMPLEMENTATION)放在编译器FLAG里即可与这个文件等效替换，
// 这个文件存在的意义：
//      节省时间。代码只在这里将宏展开，构建系统会将函数的实现先编译，可以避免每一次的改动都要重新编译函数的实现(我的设备运行很慢)
/*
-DSTBI_ONLY_BMP -DSTBI_ONLY_PNG -DSTBI_ONLY_JPEG 
*/