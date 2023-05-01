// Last edit time is 2023-01-13 23:50:59. By zhiji 
#if !defined(ZJ_SHADER_HPP)
#define ZJ_SHADER_HPP

// 为C++的包装
#ifdef __cplusplus
#include "zj_shader.h"
#include "../common/zj_string.hpp"
class zj_shader {
    public:
        zj_shader();
        zj_shader(const zj_string& vspath, const zj_string& fspath, const zj_string& vsstr, const zj_string& fsstr);
        ~zj_shader();
        void create(const zj_string& vspath, const zj_string& fspath, const zj_string& vsstr, const zj_string& fsstr);
        void destroy();
        void use() const;
        GLuint id() const;
        void set(const zj_string& uniform_name, GLint value) const;
//         void set(const zj_string& uniform_name, GLint value1, GLint value2);
//         void set(const zj_string& uniform_name, GLint value1, GLint value2, GLint value3);
//         void set(const zj_string& uniform_name, GLint value1, GLint value2, GLint value3, GLint value4);
        void set(const zj_string& uniform_name, GLuint value) const;
//         void set(const zj_string& uniform_name, GLuint value1, GLuint value2);
//         void set(const zj_string& uniform_name, GLuint value1, GLuint value2, GLuint value3);
//         void set(const zj_string& uniform_name, GLuint value1, GLuint value2, GLuint value3, GLuint value4);
        void set(const zj_string& uniform_name, GLfloat value) const;
//         void set(const zj_string& uniform_name, GLfloat value1, GLfloat value2);
//         void set(const zj_string& uniform_name, GLfloat value1, GLfloat value2, GLfloat value3);
//         void set(const zj_string& uniform_name, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);
//         void set(const zj_string& uniform_name, GLint n, GLsizei count, GLint   values[]);
//         void set(const zj_string& uniform_name, GLint n, GLsizei count, GLuint  values[]);
//         void set(const zj_string& uniform_name, GLint n, GLsizei count, GLfloat values[]);
    private:
        GLuint shader_program = 0;
};
#endif // __cplusplus

#endif
