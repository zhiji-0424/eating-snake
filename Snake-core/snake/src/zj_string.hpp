// Last edit time is 2023-01-13 23:50:36. By zhiji 
#if !defined(ZJ_STRING_HPP)
#define ZJ_STRING_HPP

#include <stdlib.h>
#include <string.h>
#include "zj_logger.h"

class zj_string {
    public:
        zj_string();
        zj_string(char ch);
        zj_string(const char* str);
        zj_string(const zj_string& str);
        void create(const char* str);

        ~zj_string();

        zj_string& operator=(const zj_string& str);
        zj_string& operator+=(const zj_string& str);
        zj_string operator+(const zj_string& str);
        bool operator==(const zj_string& str);

        inline const char* c_str() const {return data;}
        inline size_t size() const {return length;}

        static char* malloc_text(size_t size);
    private:
        void destroy(); //销毁 this->data
        char* data = nullptr;
        size_t length = 0;
};

#endif //ZJ_STRING_HPP
