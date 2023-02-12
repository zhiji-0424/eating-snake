// Last edit time is 2023-01-13 23:50:14. By zhiji 
// UTF-8 转 UTF-16-BE : 目前仅仅支持0x0000-0xffff的Unicode

#if !defined(UTF8_TO_UTF16_H)
#define UTF8_TO_UTF16_H

/* 先获取 u8c 码， 再转换成 u16c
 * 通过获取字节数量来知道这个字符有多少字节 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#include <stdint.h>

// UTF-8 码
typedef uint32_t zj_u8c;
// UTF-16 码
typedef uint16_t zj_u16c;

// 获取字节数量
int32_t zjutu_get_byte_num_from_char(uint8_t first_byte);
int32_t zjutu_get_byte_num_from_code(zj_u8c utf8_code);

// 从字符串获取UTF-8码 每个位置依次拼接 ("你" : 0xe4 0xbd 0xa0 => 0xe4bda0)
zj_u8c zjutu_get_u8c_from_cstr(const uint8_t* utf8_str);

// UTF-8 字符转换成 UTF16 字符 (e.g. "你": 0xe4bda0->0x4f60)
zj_u16c zjutu_u8c_to_u16c(zj_u8c utf8_code);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UTF8_TO_UTF16_H
