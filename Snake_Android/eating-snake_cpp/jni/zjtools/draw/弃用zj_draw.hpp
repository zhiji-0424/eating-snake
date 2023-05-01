// Last edit time is 2023-01-13 23:51:09. By zhiji 
#if !defined(ZJ_DRAW_HPP)
#define ZJ_DRAW_HPP

/* 创建一个 zj_pen 对象， 并在窗口更新(INIT/TERM)时调用init/destroy， 再进行绘图*/

#include <GLES3/gl3.h>
#include <stdint.h>
#include "zj_shader.hpp"
#include "zj_android_native_app_glue.hpp"   // 需要 "ZJ_USES_ANDROID_NATIVE_APP_GLUE"
#include "../common/zj_string.hpp"
#include "../common/zj_logger.h"

#include "stb_image.h"
class zj_texture {
    private:
        GLuint texture = GL_NONE;
        int32_t nrChannels, w, h;
        GLfloat tex_coor[8];
    public:
        zj_texture() : texture(GL_NONE), nrChannels(0), w(0), h(0) {}

        /* bx, by, ex, ey -> begin(left top).xy, end(right bottom).xy 预先设置纹理坐标 */
        // 创建并全部截取
        zj_texture(const zj_string& file_path) {this->create(file_path);}

        // 创建时选择截取(使用笛卡尔直角坐标系)
        zj_texture(const zj_string& file_path, const zj_rect& rect) {this->create(file_path, rect);}

        ~zj_texture() {this->destroy();}

        // 截取要绘制的那部分纹理(使用笛卡尔直角坐标系)
        void set_tex_coor(const zj_rect& rect);

        void create(const zj_string& file_path) {this->create(file_path, zj_rect(0, 0, 0, 0));}
        void create(const zj_string& file_path, const zj_rect& rect);
        void destroy()
        {
            glDeleteTextures( 1, &(this->texture) );
            this->texture = GL_NONE;
        }

        const GLfloat* get_tex_coor() const {return tex_coor;}
        GLuint get_texture() const {return this->texture;}
        int32_t get_w() const {return this->w;}
        int32_t get_h() const {return this->h;}
        void get_wh(int32_t& w_, int32_t& h_) const {w_=this->w;h_=this->h;}
};

typedef zj_vec4f zj_color;
typedef zj_vec2i zj_point;

// 以左上角为原点，水平向右、竖直向下分别为正方向
class zj_pen {
    private:
        bool inited = false;
        int32_t scrw, scrh;
        GLuint shader;
        // GLuint shader_circle; // 画圆时使用
        GLuint shader_edge_circle; // 画圆环时使用
        zj_color pen_color;
        GLuint shader_tex;
        zj_color tex_color;
        /* 0+++++1
           +-----+
           2+++++3 */
        GLfloat rect_vs[8];/* = {
            0, 0,  // 0
            0, 0,  // 1
            0, 0,  // 2
            0, 0   // 3
        };*/
        GLuint indices[6];/* = {
            0, 2, 1,
            2, 3, 1
        };*/
        GLfloat line_vs[4];/* = {
            0, 0,    // (x1, y1)
            0, 0     // (x2, y2)
        };*/

    public:
        zj_pen() : inited(false), scrw(0), scrh(0), shader(GL_NONE), shader_edge_circle(GL_NONE), shader_tex(GL_NONE)
        {
            pen_color = zj_color(1, 1, 1, 1);
            tex_color = zj_color(1, 1, 1, 1);
            indices[0] = 0;
            indices[1] = 2;
            indices[2] = 1;
            indices[3] = 2;
            indices[4] = 3;
            indices[5] = 1;
        }

        ~zj_pen() {this->destroy();}

        // 初始化绘图(必须)
        void init(void);
        void destroy(void);

        // 是否初始化完成
        bool is_inited(void) const {return this->inited;}

        /* 1
        -1 0 1
          -1 */
        // 像素坐标转换成 OpenGL坐标(屏幕中心为原点)
        // X // 根据 (float)(pixelx-scrw/2)/(pixelx/2);
        float p2glx(int32_t pixelx) const {return (float)2*pixelx/this->scrw-1;}
        // Y // 根据 -(float)(pixely-scrh/2)/(pixely/2);
        float p2gly(int32_t pixely) const {return -(float)2*pixely/this->scrh+1;}

        // 设置内部计算使用的屏幕大小(建议每次绘图前调用)
        void set_scrwh(int32_t w, int32_t h) {this->scrw=w;this->scrh=h;}

        // 设置画笔颜色
        void set_color(const zj_color& color) {pen_color.set(color.x, color.y, color.x, color.w);}

        // 用颜色清空绘图区
        void clear(const zj_color& color);

        // 画点
        // void point(int32_t x, int32_t y);

        // 画线
        void line(const zj_point& start_point, const zj_point& stop_point);

        // 画矩形框
        void edge_rect(const zj_rect& rect);

        // 画填充矩形，(x,y)为左上角坐标，(w,h)为矩形宽高
        void fill_rect(const zj_rect& rect);

        // 画空心圆, x, y, radius, line_width (空心部分的半径=radius-line_width/2)
        void edge_circle(const zj_point& position, int32_t radius, int32_t line_width);

        // 画实心圆, x, y, radius
        void fill_circle(const zj_point& position, int32_t radius);

        // 设置纹理混合颜色
        void set_tex_color(const zj_color& color) {tex_color.set(color.x, color.y, color.x, color.w);}
        // 画出纹理(参数是屏幕坐标，所以在画之前先截取)
        void texture(const zj_texture& texture, const zj_rect& rect);
};

#endif //ZJ_DRAW_HPP
