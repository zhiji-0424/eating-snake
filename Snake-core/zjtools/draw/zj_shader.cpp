#include "zj_shader.hpp"

// 如果决定使用C编写，那么这个文件扩展名可以改为‘.c’

#define LOGE(...) zj_logger_e("zj_shader", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DEBUG_SHADER)
#  define LOGD(...) zj_logger_d("zj_shader", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

#define INFO_LOG_SIZE 1000

// 顶点着色器
static GLuint create_vertex_shader(const char* vsstr)
{
    if (!vsstr) {
        LOGE("create_vertex_shader: vertex shader str is null");
        return 0;
    }
    int success;
    char info_log[INFO_LOG_SIZE];
    // vertex_shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vsstr, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, INFO_LOG_SIZE, nullptr, info_log);
        LOGE("create_vertex_shader:\n\n[%s]Source:[\n%s\n]\nID:(%d)\n", info_log, vsstr, vertex_shader);
    }
    return vertex_shader;
}

// 片段着色器
static GLuint create_fragment_shader(const char* fsstr)
{
    if (!fsstr) {
        LOGE("create_fragment_shader: fragment shader str is null");
        return 0;
    }
    int success;
    char info_log[INFO_LOG_SIZE];
    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fsstr, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFO_LOG_SIZE, nullptr, info_log);
        LOGE("create_fragment_shader:\n\n[%s]Source:[\n%s\n]\nID:(%d)\n", info_log, fsstr, fragment_shader);
    }
    return fragment_shader;
}

// 着色器程序
static GLuint link_program(GLuint vertex_shader, GLuint fragment_shader)
{
    int success;
    char info_log[INFO_LOG_SIZE];
    // link shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, INFO_LOG_SIZE, nullptr, info_log);
        LOGE("link_program:\n[%s]\nID:(%d)\n", info_log, shader_program);
    }
    return shader_program;
}

// 创建方式1：通过着色器源码
static inline GLuint create_program_1(const char* vsstr, const char* fsstr)
{
    GLuint vertex_shader   = create_vertex_shader(vsstr);
    GLuint fragment_shader = create_fragment_shader(fsstr);
    GLuint shader_program  = link_program(vertex_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

// 获取文件大小
static inline size_t get_file_size(FILE* f)
{
    if (f) {
        size_t size = 0;
        fpos_t pos = 0;
        fgetpos(f, &pos);    // 记录位置
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fsetpos(f, &pos);    // 恢复位置
        return size;
    }
    return 0;
}

// 申请存储文本的内存区域(自动补结尾'\0')
static inline char* malloc_text_buf(long size)
{
    size += 1;  // 别忘了结尾的'\0'
    char* buf = (char*)malloc(size);
    if (!buf) {
        LOGE("malloc_text_buf: out of memory! requested size is %ld", size);
        return nullptr;
    }
    memset(buf, '\0', size);
    return buf;
}

// 从路径读取着色器文件
static char* read_shader_file(const char* path)
{
    if (path == nullptr)
        return nullptr;
    FILE* f = fopen(path, "r");
    if (!f) {
        LOGE("read_shader_file: file open failed! "
             "file path is :(%s)", path);
        return nullptr;
    }
    size_t file_size = get_file_size(f);
//    LOGD("read_shader_file: file size is %ld", file_size);
    char* res = malloc_text_buf(file_size);
    if (!res) {
        LOGE("read_shader_file: out of momery! "
             "requested size is %ld", file_size);
        fclose(f);
        return nullptr;
    }
    int c = 0;
    size_t pos = 0;
    while ((c = fgetc(f)) != EOF) {
        res[pos++] = c;
    }
    fclose(f);
    return res;
}

// 创建方式2：通过着色器文件路径
static GLuint create_program_2(const char* vspath, const char* fspath)
{
    char* vsstr = read_shader_file(vspath);
    char* fsstr = read_shader_file(fspath);
    GLuint shader_program = 0;
    if (vsstr != nullptr && fsstr != nullptr) {
        shader_program = create_program_1(vsstr, fsstr);
    } else {
        LOGE("create_program_2: vss or fss is null! "
             "vertex shader str is %p, "
             "fragment shader str is %p", vsstr, fsstr);
    }
    free(vsstr);
    free(fsstr);
    return shader_program;
}

// 创建的方法：自动选择(使用0或者null填入不需要的参数)
GLuint zj_shader_create_program(const char* vspath, const char* fspath, const char* vsstr, const char* fsstr)
{
    GLuint shader_program = 0;
    if (!vspath && !fspath && vsstr && fsstr) {
        shader_program = create_program_1(vsstr, fsstr);
    } else if (vspath && fspath && !vsstr && !fsstr) {
        shader_program = create_program_2(vspath, fspath);
    } else {
        LOGE("create_program:arguments are incorrect! "
             "vspath:%p fspath:%p vsstr:%p fsstr:%p",
             vspath, fspath, vsstr, fsstr);
    }
    return shader_program;
}

// 为C++的包装
#ifdef __cplusplus
zj_shader::zj_shader()
{
    this->shader_program = 0;
}

zj_shader::~zj_shader()
{
    this->destroy();
}

zj_shader::zj_shader(const zj_string& vspath, const zj_string& fspath, const zj_string& vsstr, const zj_string& fsstr)
{
    this->create(vspath, fspath, vsstr, fsstr);
}

void zj_shader::create(const zj_string& vspath, const zj_string& fspath, const zj_string& vsstr, const zj_string& fsstr)
{
    this->shader_program = zj_shader_create_program(vspath.c_str(), fspath.c_str(), vsstr.c_str(), fsstr.c_str());
}

void zj_shader::destroy()
{
    glDeleteProgram(this->shader_program);
    this->shader_program = GL_NONE;
}

void zj_shader::use() const
{
    glUseProgram(this->shader_program);
}

GLuint zj_shader::id() const
{
    return this->shader_program;
}

void zj_shader::set(const zj_string& uniform_name, GLint value) const
{
    glUniform1i(glGetUniformLocation(this->shader_program, uniform_name.c_str()), value);
}
// void zj_shader::set(const zj_string& uniform_name, GLint value1, GLint value2);
// void zj_shader::set(const zj_string& uniform_name, GLint value1, GLint value2, GLint value3);
// void zj_shader::set(const zj_string& uniform_name, GLint value1, GLint value2, GLint value3, GLint value4);

void zj_shader::set(const zj_string& uniform_name, GLuint value) const
{
    glUniform1ui(glGetUniformLocation(this->shader_program, uniform_name.c_str()), value);
}
// void zj_shader::set(const zj_string& uniform_name, GLuint value1, GLuint value2);
// void zj_shader::set(const zj_string& uniform_name, GLuint value1, GLuint value2, GLuint value3);
// void zj_shader::set(const zj_string& uniform_name, GLuint value1, GLuint value2, GLuint value3, GLuint value4);

void zj_shader::set(const zj_string& uniform_name, GLfloat value) const
{
    glUniform1f(glGetUniformLocation(this->shader_program, uniform_name.c_str()), value);
}
// void zj_shader::set(const zj_string& uniform_name, GLfloat value1, GLfloat value2);
// void zj_shader::set(const zj_string& uniform_name, GLfloat value1, GLfloat value2, GLfloat value3);
// void zj_shader::set(const zj_string& uniform_name, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);

#endif // __cplusplus