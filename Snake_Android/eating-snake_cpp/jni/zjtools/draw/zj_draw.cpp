#include "zj_draw.hpp"

#define LOGE(...) zj_logger_e("zj_draw", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DEBUG_DRAW)
#  define LOGD(...) zj_logger_d("zj_draw", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif


void zj_texture::create(const zj_string& file_path, const zj_rect& rect)
{
    glGenTextures( 1, &(this->texture) );
    glBindTexture(GL_TEXTURE_2D, this->texture);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // GL_REPEAT GL_CLAMP_TO_EDGE
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // GL_LINEAR GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(file_path.c_str(), &(this->w), &(this->h), &(this->nrChannels), 0);
    int32_t fmt = 0;
    switch (this->nrChannels) {
        case 3:
            fmt = GL_RGB;
            break;
        case 4:
            fmt = GL_RGBA;
            break;
        default:
            LOGE("zj_texture::create(): nrChannels=%d", this->nrChannels);
            break;
    }
    
    if (data != nullptr && fmt != 0) {
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, this->w, this->h, 0, fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    else {
        if (data == nullptr) {
            LOGE("zj_texture::create to load texture! (data=null) "
                 "File path is (%p)%s.", file_path.c_str(), file_path.c_str());
        }
        else {
            LOGE("zj_texture::create(): Wrong in getting nrChannels! nrChannels=%d", this->nrChannels);
        }
    }
    stbi_image_free(data);
    
    this->set_tex_coor(rect);
}

void zj_texture::set_tex_coor(const zj_rect& rect)
{
    // 判断面积是不是0
    if (rect.w * rect.h == 0) {
        // 使用整张图片
        this->tex_coor[0] = 0.0f;
        this->tex_coor[1] = 1.0f;
        this->tex_coor[2] = 1.0f;
        this->tex_coor[3] = 1.0f;
        this->tex_coor[4] = 0.0f;
        this->tex_coor[5] = 0.0f;
        this->tex_coor[6] = 1.0f;
        this->tex_coor[7] = 0.0f;
    } else {
        // 使用选定的部分
        this->tex_coor[0] = (float)rect.x/this->w;
        this->tex_coor[1] = (float)rect.y/this->h;
        this->tex_coor[2] = (float)(rect.x+rect.w)/this->w;
        this->tex_coor[3] = (float)rect.y/this->h;
        this->tex_coor[4] = (float)rect.x/this->w;
        this->tex_coor[5] = (float)(rect.y-rect.h)/this->h;
        this->tex_coor[6] = (float)(rect.x+rect.w)/this->w;
        this->tex_coor[7] = (float)(rect.y-rect.h)/this->h;
    }
}

static const char* vertex_shader_str = 
"#version 300 es\n"
"layout(location = 0) in vec4 Position;\n""\n"

"void main()\n"
"{\n"
"    gl_Position = Position;\n"
"}";

static const char* fragment_shader_str =
"#version 300 es\n"
"precision mediump float;\n""\n"

"uniform vec4 VertexColor;\n"
"out vec4 FragColor;\n""\n"

"void main()\n"
"{\n"
"    FragColor = VertexColor;\n"
"}";

/*//有替代方法了
static const char* fragment_shader_str_circle =
"#version 300 es\n"
"precision mediump float;\n""\n"

"uniform vec4 VertexColor;\n"
"uniform float ScreenSize;\n"
"uniform vec2 CirclePosition;\n"
"uniform float CircleRadius;\n"
"out vec4 FragColor;\n""\n"

"// 这里使用左下角为原点的平面直角坐标系\n"
"float circle(vec2 circle_coord, float circle_radius)\n"
"{\n"
"    // 点在圆外返回0.0，在圆内返回1.0\n"
"    return step(distance(gl_FragCoord.xy/ScreenSize, circle_coord) - circle_radius, 0.0);\n"
"}\n""\n"

"void main()\n"
"{\n"
"    float a = circle(CirclePosition, CircleRadius);\n"
"    FragColor = vec4(VertexColor.xyz, a);\n"
"}";
*/

static const char* fragment_shader_str_circle_line =
"#version 300 es\n"
"precision mediump float;\n""\n"

"uniform vec4 VertexColor;\n"
"uniform float ScreenSize;\n"
"uniform vec2 CirclePosition;\n"
"uniform float CircleRadius;\n"
"uniform float CircleLineWidth;\n"
"out vec4 FragColor;\n""\n"

"// 这里使用左下角为原点的平面直角坐标系\n"
"// 让半径位于圆环线中\n"
"float circle_line(vec2 p, float r, float lw)\n"
"{\n"
"    // 点在圆环外返回0.0，在圆环内返回1.0\n"
"    float d = distance(gl_FragCoord.xy/ScreenSize, p) - r;\n"
"    if (d > -lw/2.0 && d < lw/2.0)\n"
"        return 1.0;\n"
"    return 0.0;\n"
"}\n""\n"

"void main()\n"
"{\n"
"    float a = circle_line(CirclePosition, CircleRadius, CircleLineWidth);\n"
"    FragColor = vec4(VertexColor.xyz, a);\n"
"}";

static const char* vertex_shader_str_tex = 
"#version 300 es\n"
"layout(location = 0) in vec4 Position;\n"
"layout(location = 1) in vec2 TexCoor_in;\n"
"out vec2 TexCoor;\n""\n"

"void main()\n"
"{\n"
"    gl_Position = Position;\n"
"    TexCoor = TexCoor_in;\n"
"}";

static const char* fragment_shader_str_tex = 
"#version 300 es\n"
"precision mediump float;\n""\n"

"in  vec2 TexCoor;\n"
"out vec4 FragColor;\n"
"uniform sampler2D spl;\n"
"uniform vec4 TexColor;\n""\n"

"void main()\n"
"{\n"
"    FragColor = texture(spl, TexCoor) * TexColor;\n"
"}";


void zj_pen::clear(const zj_color& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void zj_pen::init()
{
    this->inited = true;
    glDisable(GL_DEPTH_TEST);
    this->shader = zj_shader_create_program(0, 0, vertex_shader_str, fragment_shader_str);
    //shader_circle = zj_shader_create_program(0, 0, vertex_shader_str, fragment_shader_str_circle);
    this->shader_edge_circle = zj_shader_create_program(0, 0, vertex_shader_str, fragment_shader_str_circle_line);
    this->set_color( zj_color(1, 1, 1, 1) );
    this->shader_tex = zj_shader_create_program(0, 0, vertex_shader_str_tex, fragment_shader_str_tex);
    this->set_tex_color( zj_color(1, 1, 1, 1) );
}

void zj_pen::destroy()
{
    inited = false;
    glDeleteProgram(this->shader);
    //glDeleteProgram(shader_circle);
    glDeleteProgram(this->shader_edge_circle);
    glDeleteProgram(this->shader_tex);
}

/*void draw_point(int32_t x, int32_t y)
{
    // 也可以用线的来存储点TUDO
    glUseProgram(shader);
    GLint location = glGetUniformLocation(shader, "VertexColor");
    glUniform4f(location, r, g, b, a);
    this->line_vs[0] = p2glx(x);
    this->line_vs[1] = p2gly(y);
    this->line_vs[2] = p2glx(x+10);
    this->line_vs[3] = p2gly(y+10);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, this->line_vs);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, 2);
}*/

void zj_pen::line(const zj_point& start, const zj_point& end)
{
    glUseProgram(shader);
    GLint location = glGetUniformLocation(shader, "VertexColor");
    glUniform4f(location, pen_color.x, pen_color.y, pen_color.z, pen_color.w);
    this->line_vs[0] = this->p2glx(start.x);
    this->line_vs[1] = this->p2gly(start.y);
    this->line_vs[2] = this->p2glx(end.x);
    this->line_vs[3] = this->p2gly(end.y);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, this->line_vs);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void zj_pen::fill_rect(const zj_rect& rect)
{
    glUseProgram(shader);
    GLint location = glGetUniformLocation(shader, "VertexColor");
    glUniform4f(location, pen_color.x, pen_color.y, pen_color.z, pen_color.w);
    this->rect_vs[0] = this->p2glx(rect.x);
    this->rect_vs[1] = this->p2gly(rect.y);
    this->rect_vs[2] = this->p2glx(rect.x+rect.w);
    this->rect_vs[3] = this->p2gly(rect.y);
    this->rect_vs[4] = this->p2glx(rect.x);
    this->rect_vs[5] = this->p2gly(rect.y+rect.h);
    this->rect_vs[6] = this->p2glx(rect.x+rect.w);
    this->rect_vs[7] = this->p2gly(rect.y+rect.h);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, this->rect_vs);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, this->indices);
}

void zj_pen::edge_rect(const zj_rect& rect)
{
    // 减1 : 为了填补框框左上角的缺口
    this->line( zj_point(rect.x-1, rect.y), zj_point(rect.x+rect.w, rect.y) );    // 上横
    this->line( zj_point(rect.x, rect.y), zj_point(rect.x, rect.y+rect.h) );      // 左竖
    this->line( zj_point(rect.x, rect.y+rect.h), zj_point(rect.x+rect.w, rect.y+rect.h) );    // 下横
    this->line( zj_point(rect.x+rect.w, rect.y), zj_point(rect.x+rect.w, rect.y+rect.h) );    // 右竖
}

void zj_pen::fill_circle(const zj_point& p, int32_t radius)
{
/*  glUseProgram(shader_circle);
    // 要转成以左下角为原点的坐标系    
    int32_t screen_size = (scrw > scrh) ? scrh : scrw;  // 以屏幕短的那边为标准(长的也可以)
    GLint location = glGetUniformLocation(shader_circle, "VertexColor");
    glUniform4f(location, r, g, b, a);
    location = glGetUniformLocation(shader_circle, "ScreenSize");
    glUniform1f(location, screen_size);
    location = glGetUniformLocation(shader_circle, "CirclePosition");
    glUniform2f(location, (float)x/screen_size, (float)(scrh-y)/screen_size);
    location = glGetUniformLocation(shader_circle, "CircleRadius");
    glUniform1f(location, (float)radius/screen_size);
    this->rect_vs[0] = this->p2glx(x-radius);
    this->rect_vs[1] = this->p2gly(y-radius);
    this->rect_vs[2] = this->p2glx(x+radius);
    this->rect_vs[3] = this->p2gly(y-radius);
    this->rect_vs[4] = this->p2glx(x-radius);
    this->rect_vs[5] = this->p2gly(y+radius);
    this->rect_vs[6] = this->p2glx(x+radius);
    this->rect_vs[7] = this->p2gly(y+radius);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, this->rect_vs);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
*/
    this->edge_circle(p, radius/2, radius);
}

void zj_pen::edge_circle(const zj_point& p, int32_t radius, int32_t line_width)
{
    glUseProgram(shader_edge_circle);

    // 要转成以左下角为原点的坐标系    
    int32_t screen_size = (scrw > scrh) ? scrh : scrw;  // 以屏幕短的那边为标准(长的也可以)
    GLint location = glGetUniformLocation(shader_edge_circle, "VertexColor");
    glUniform4f(location, pen_color.x, pen_color.y, pen_color.z, pen_color.w);
    location = glGetUniformLocation(shader_edge_circle, "ScreenSize");
    glUniform1f(location, screen_size);
    location = glGetUniformLocation(shader_edge_circle, "CirclePosition");
    glUniform2f(location, (float)p.x/screen_size, (float)(scrh-p.y)/screen_size);
    location = glGetUniformLocation(shader_edge_circle, "CircleRadius");
    glUniform1f(location, (float)radius/screen_size);
    location = glGetUniformLocation(shader_edge_circle, "CircleLineWidth");
    glUniform1f(location, (float)line_width/screen_size);
    this->rect_vs[0] = this->p2glx(p.x-radius-line_width/2);
    this->rect_vs[1] = this->p2gly(p.y-radius-line_width/2);
    this->rect_vs[2] = this->p2glx(p.x+radius+line_width/2);
    this->rect_vs[3] = this->p2gly(p.y-radius-line_width/2);
    this->rect_vs[4] = this->p2glx(p.x-radius-line_width/2);
    this->rect_vs[5] = this->p2gly(p.y+radius+line_width/2);
    this->rect_vs[6] = this->p2glx(p.x+radius+line_width/2);
    this->rect_vs[7] = this->p2gly(p.y+radius+line_width/2);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, this->rect_vs);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, this->indices);
}

// 使用前面rect的矩形，纹理坐标也对应矩形四角，顶点顺序也是
void zj_pen::texture(const zj_texture& texture, const zj_rect& rect)
{
    glUseProgram(shader_tex);
    GLint location = glGetUniformLocation(shader_tex, "TexColor");
    glUniform4f(location, tex_color.x, tex_color.y, tex_color.z, tex_color.w);
    this->rect_vs[0] = this->p2glx(rect.x);
    this->rect_vs[1] = this->p2gly(rect.y);
    this->rect_vs[2] = this->p2glx(rect.x + rect.w);
    this->rect_vs[3] = this->p2gly(rect.y);
    this->rect_vs[4] = this->p2glx(rect.x);
    this->rect_vs[5] = this->p2gly(rect.y + rect.h);
    this->rect_vs[6] = this->p2glx(rect.x + rect.w);
    this->rect_vs[7] = this->p2gly(rect.y + rect.h);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, this->rect_vs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, texture.get_tex_coor());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    GLint lct = glGetUniformLocation(shader_tex, "spl");
    glUniform1i(lct, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, texture.get_texture() );
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, this->indices);
}
