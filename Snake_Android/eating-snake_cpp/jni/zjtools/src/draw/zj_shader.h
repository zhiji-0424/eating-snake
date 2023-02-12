#if !defined(ZJ_SHADER_H)
#define ZJ_SHADER_H

// C语言版本

#include <GLES3/gl3.h>
#include <stdio.h> 
#include <stdlib.h>
#include "../common/zj_logger.h"

#ifdef __cplusplus
extern "C"
#endif
GLuint zj_shader_create_program(const char* vspath, const char* fspath, const char* vsstr, const char* fsstr);
// 创建的方法：自动选择(使用0或者null填入不需要的参数)

#endif