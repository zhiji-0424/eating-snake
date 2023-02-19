#if !defined(ZJ_RECT_HPP)
#define ZJ_RECT_HPP

#include <stdint.h>

struct zj_rect_st {
    int32_t x, y, w, h;

    zj_rect_st()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    zj_rect_st(int32_t x_, int32_t y_, int32_t w_, int32_t h_)
    {
        x = x_;
        y = y_;
        w = w_;
        h = h_;
    }
    
    void set(int32_t x_, int32_t y_, int32_t w_, int32_t h_)
    {
        x = x_;
        y = y_;
        w = w_;
        h = h_;
    }
};

typedef zj_rect_st zj_rect;

#endif //ZJ_RECT_HPP