#if !defined(ZJ_VECS_HPP)
#define ZJ_VECS_HPP

#include <stdint.h>

// 2 int
typedef struct zj_vec2i_st {
    int32_t x, y;

    zj_vec2i_st()
    {
        x = 0;
        y = 0;
    }

    zj_vec2i_st(int32_t x_, int32_t y_)
    {
        x = x_;
        y = y_;
    }

    void set(int32_t x_, int32_t y_)
    {
        x = x_;
        y = y_;
    }
} zj_vec2i;

// 3 int
typedef struct zj_vec3i_st {
    int32_t x, y, z;

    zj_vec3i_st()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    zj_vec3i_st(int32_t x_, int32_t y_, int32_t z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    void set(int32_t x_, int32_t y_, int32_t z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
} zj_vec3i;

// 4 int
typedef struct zj_vec4i_st {
    int32_t x, y, z, w;

    zj_vec4i_st()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    zj_vec4i_st(int32_t x_, int32_t y_, int32_t z_, int32_t w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }

    void set(int32_t x_, int32_t y_, int32_t z_, int32_t w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }
} zj_vec4i;


// 2 float
typedef struct zj_vec2f_st {
    float x, y;

    zj_vec2f_st()
    {
        x = 0;
        y = 0;
    }

    zj_vec2f_st(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

    void set(float x_, float y_)
    {
        x = x_;
        y = y_;
    }
} zj_vec2f;

// 3 float
typedef struct zj_vec3f_st {
    float x, y, z;

    zj_vec3f_st()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    zj_vec3f_st(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    void set(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
} zj_vec3f;

// 4 float
typedef struct zj_vec4f_st {
    float x, y, z, w;

    zj_vec4f_st()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    zj_vec4f_st(float x_, float y_, float z_, float w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }

    void set(float x_, float y_, float z_, float w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }
} zj_vec4f;

#endif //ZJ_VECS_HPP