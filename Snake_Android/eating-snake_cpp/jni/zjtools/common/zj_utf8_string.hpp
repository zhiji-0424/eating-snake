#if !define(ZJ_UTF8_STRING_HPP)
#define ZJ_UTF8_STRING_HPP

#include "zj_string.hpp"

class zj_utf8_string {
    private:
        void* data; // -> zj_utf8_string

    public:
        zj_utf8_string();
        zj_utf8_string(char ch);
        zj_utf8_string(const char* origin_str);
        zj_utf8_string(const zj_string& origin_str);
        zj_utf8_string(const zj_utf8_string& origin_str);
        ~zj_utf8_string();

        void create(const char* origin_str);
        void destroy();

        int32_t insert_str(int32_t index, const char* str);   // 在 index 前面插入, 0:成功;非0:失败,并有错误码
        int32_t delete_str(int32_t index);  // 删除索引为 index 的字符

        char* get_string() const;
        char* get_character(int32_t index) const;   // 获取单个 UTF-8 字符，索引从 0 开始
        int32_t get_character_utf8code (int32_t index) const;
        int32_t get_character_utf16code(int32_t index) const;
        
};

#endif //end of ZJ_UTF8_STRING_HPP