#ifndef GAME_HPP
#define GAME_HPP

#include <android_native_app_glue.h>
#include <stdint.h>
#include <tools/String.hpp>
#include <tools/draw/glue.hpp>

class Game {
    public:
        Game();
        ~Game();
        void init_graphics();
        void term_graphics();
        inline bool is_inited() const {return inited;};
        void init(const char* externalDataPath);
        void loop();
        void draw(int32_t scrw, int32_t scrh, egl_state egl);
        int32_t handle_input(struct android_app* app, AInputEvent* event);

    private:
        bool inited = false;
        bool drawing = false;
        String data_path;
};


#endif