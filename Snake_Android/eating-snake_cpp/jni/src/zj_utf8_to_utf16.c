// UTF-8 转 UTF-16-BE : 目前仅仅支持0x0000-0xffff的Unicode

/* 1. 0000 - 007f           | 0xxx xxxx
 * 2. 0080 - 07ff           | 110x xxxx  10xx xxxx
 * 3. 0800 - ffff           | 1110 xxxx  10xx xxxx  10xx xxxx
 * 4. 0001 0000 - 0010 ffff | 1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx
 * 5. 0020 0000 - 03ff ffff | 1111 10xx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx
 * 6. 0400 0000 - 7fff ffff | 1111 110x  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx */

#include "zj_utf8_to_utf16.h"
#include "zj_logger.h"

#define LOGE(...) zj_logger_e("zj_utf8_to_utf16", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DEBUG_UTU)
#  define LOGD(...) zj_logger_d("zj_utf8_to_utf16", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

/* (1000 0000) : 0x80
 * (0010 0000) : 0x20
 * (0001 0000) : 0x10
 * (0000 1000) : 0x08
 * (0000 0100) : 0x04
 * (0000 0010) : 0x02 */

// 用检查0的位置的方法来计算编码的字节数
int32_t zjutu_get_byte_num_from_char(uint8_t first_byte)
{
    int32_t num = 1;    //如果传入的编码错误，则以1个字节记

    if ( (first_byte & 0x80) == 0 ) {
        num = 1;
    } else if ( (first_byte & 0x20) == 0 ) {
        num = 2;
    } else if ( (first_byte & 0x10) == 0 ) {
        num = 3;
    } /*else if ( (first_byte & 0x08) == 0 ) {
        num = 4;
    } else if ( (first_byte & 0x04) == 0 ) {
        num = 5;
    } else if ( (first_byte & 0x02) == 0 ) {
        num = 6;
    }*/ else {
        LOGE("zjutu_get_byte_num_from_char(): 该字节数超过了范围，不是合法字符");
    }

    return num;
}

// 将开头的字节传给另一个函数来完成功能
int32_t zjutu_get_byte_num_from_code(zj_u8c utf8_code)
{
    uint8_t first_byte = (utf8_code & 0x00ff0000) >> 16;
    if (first_byte == 0) {
        first_byte = (utf8_code & 0x0000ff00) >> 8;
        if (first_byte == 0) {
            first_byte = utf8_code & 0x000000ff;
        }
    }
    return zjutu_get_byte_num_from_char(first_byte);
}

// 从字符串获取UTF-8码 每个位置依次拼接(0xe4 0xbd 0xa0 => 0xe4bda0)
zj_u8c zjutu_get_u8c_from_cstr(const uint8_t* utf8_str)
{
    if (utf8_str == NULL) {
        return 0;
    }
    int32_t num = zjutu_get_byte_num_from_char(utf8_str[0]);
    zj_u8c code = 0;
    switch (num) {
        case 1:
            code = utf8_str[0];
            break;
        case 2:
            if ( (utf8_str+1) != NULL ) {
                code = (utf8_str[0] << 8) + utf8_str[1];
            }
            break;
        case 3:
            if ( (utf8_str+1 != NULL) && (utf8_str+2 != NULL) ) {
                code = (utf8_str[0] << 16) + (utf8_str[1] << 8) + utf8_str[2];
            }
            break;
        default:
            LOGE("zjutu_get_u8c_from_cstr: 超过可处理范围1~3(%d)", num);
            break;
    }
    return code;
}

/*  (0000 0000 0000 0000 0011 1111) : 0x0003f
 *  (0000 0000 0000 0011 0000 0000) : 0x00300
 *  (0000 0000 0001 1100 0000 0000) : 0x01c00
 *  (0000 0000 0011 1111 0000 0000) : 0x03f00
 *  (0000 1111 0000 0000 0000 0000) : 0xf0000 */
//将UTF-8码转换成UTF-16码
zj_u16c zjutu_u8c_to_u16c(zj_u8c utf8_code)
{
    zj_u16c code = 0;
    int32_t num = zjutu_get_byte_num_from_code(utf8_code);
    switch (num) {
        case 1:
            code = utf8_code;
            break;
        case 2:
            code = utf8_code & 0x3f;
            code += (utf8_code & 0x0300) >> 2;
            code += (utf8_code & 0x1c00) >> 2;
            break;
        case 3:
            code = utf8_code & 0x3f;
            code += (utf8_code & 0x3f00) >> 2;
            code += (utf8_code & 0xf0000) >> 4;
            break;
        default:
            LOGE("zjutu_u8c_to_u16c: 超过范围1~3(%d)", num);
            break;
    }
    return code;
}