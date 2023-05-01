#if !defined(ZJ_UTF8_STRING_HPP)
#define ZJ_UTF8_STRING_HPP

#include "zj_logger.h"
#include "zj_string.hpp"
#include <stdlib.h>
#include <string.h>

enum zj_string_status_code {
    ok = 0,
    out_of_range,
    out_of_memory,
    string_is_null,
    string_already_had_elements
};

class zj_utf8_string {
    private:
        void* head;     // -> zj_utf8_code
        int32_t length; // 长度：字符的个数

    public:
        zj_utf8_string();
        zj_utf8_string(char ch);
        zj_utf8_string(const char* origin_str);
        zj_utf8_string(const zj_string& origin_str);
        zj_utf8_string(const zj_utf8_string& origin_str);
        ~zj_utf8_string();

        int32_t create(const char* origin_str);
        void destroy();

        int32_t insert_str(int32_t index, const char* str); // 在 index 前面插入, 返回错误码
        int32_t insert_str(int32_t index, const zj_string& str);
        int32_t insert_str(int32_t index, const zj_utf8_string& str);
        int32_t delete_str(int32_t index);                  // 删除索引为 index 的字符

        char* get_string() const;   // 把字符合并为字符串并返回
        char* get_character(int32_t index) const;   // 获取单个 UTF-8 字符，索引从 0 开始
        int32_t get_character_utf8code (int32_t index) const;
        int32_t get_character_utf16code(int32_t index) const;
        
        // 操作符重载
};

#endif //end of ZJ_UTF8_STRING_HPP