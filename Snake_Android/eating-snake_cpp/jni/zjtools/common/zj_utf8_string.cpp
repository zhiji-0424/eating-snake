#include "zj_utf8_string.hpp"

#define LOGE(...) zj_logger_e("zj_utf8_string", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DEBUG_UTF8_STRING)
#  define LOGD(...) zj_logger_d("zj_utf8_string", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

struct zj_utf8_code {
    struct zj_utf8_code* next;
    char* data;
    size_t data_size;
};

static struct zj_utf8_code*
create_node(void)
{
    size_t node_size = sizeof(struct zj_utf8_code);
    struct zj_utf8_code* node = (struct zj_utf8_code*)malloc(node_size);
    if (node != nullptr) {
        memset(node, 0, node_size);
    } else {
        LOGE("create_node(): Can not alloc memory!(requested size:%ld)", node_size);
    }
    return node;
}

static char* create_data(size_t size)
{
    return zj_string::malloc_text(size);
}

zj_utf8_string::zj_utf8_string() : head(nullptr), length(0)
{
}

zj_utf8_string::zj_utf8_string(char ch) : head(nullptr), length(0)
{
    char str[2] = { ch, '\0' };
    create(str);
}

zj_utf8_string::zj_utf8_string(const char* origin_str) : head(nullptr), length(0)
{
    create(origin_str);
}

zj_utf8_string::zj_utf8_string(const zj_string& origin_str) : head(nullptr), length(0)
{
    create(origin_str.c_str());
}

zj_utf8_string::zj_utf8_string(const zj_utf8_string& origin_str) : head(nullptr), length(0)
{
    create(origin_str.get_string());
}

zj_utf8_string::~zj_utf8_string()
{
    this->destroy();
}

int32_t zj_utf8_string::create(const char* origin_str)
{
    if (length == 0) {
        return insert_str(0, origin_str);
    }
    return string_already_had_elements;
}

void zj_utf8_string::destroy()
{
    while (delete_str(0) == ok);
}

// 在 index 前面插入, 返回错误码
/* 特殊情况：
 *  1. 元素个数为0时插入操作为新增开头元素；
 *  2. 假设有3个元素，索引为：0，1，2。在2后面插入元素要传递的 index 为 3 。
 */
int32_t zj_utf8_string::insert_str(int32_t index, const char* str)
{
    // 处理索引
    if (index < 0) {
        index = this->length + index;   // (index < 0) 是用'+'号没错
    }
    // 检查范围
    if ((0 > index) || (this->length-1 < index)) {
        LOGE("zj_utf8_string::insert_str(): out of range!");
        return out_of_range;
    }
    // 创建节点
    struct zj_utf8_code* new_node = create_node();
    if (new_node == nullptr) {
        return out_of_memory;
    } else {
        size_t len = strlen(str);
        new_node->data_size = len+1;
        new_node->data = create_data(len);
    }
    // 连接节点
    if (length == 0) {
        // 直接添加节点
        this->head = new_node;
        this->length += 1;
        new_node->next = nullptr;
    } else {
        index -= 1;
        if (index == -1) {
            // 添加为第一个节点
            new_node->next = (struct zj_utf8_code*)this->head;
            this->head = new_node;
            this->length += 1;
        } else { // 0~inf
            // 去到目标节点的前一个节点
            struct zj_utf8_code* t1 = (struct zj_utf8_code*)this->head;
            for (int32_t i = 0; i < index; ++i) {
                t1 = t1->next;
            }
            new_node->next = t1->next;
            t1->next = new_node;
            this->length += 1;
        }
    }
}

// 删除索引为 index 的字符
int32_t zj_utf8_string::delete_str(int32_t index)
{
    // 处理索引
    if (index < 0) {
        index = this->length + index;   // (index < 0) 是用'+'号没错
    }
    // 检查范围
    if ((0 > index) || (this->length-1 < index)) {
        LOGE("zj_utf8_string::insert_str(): out of range!");
        return out_of_range;
    }
    if (index == 0) {
        // 如果刚好是第一个节点
        struct zj_utf8_code* node = (struct zj_utf8_code*)this->head; // 暂时保存待删除的节点的地址
        this->head = ((struct zj_utf8_code*)this->head)->next;          // 连接
        free(node);                             // 删除节点
    } else {
        // 去到目标节点的前一个结点
        struct zj_utf8_code* t1 = (struct zj_utf8_code*)this->head;
        for (int32_t i = 0; i < index-1; ++i) {
            t1 = t1->next;
        }
        struct zj_utf8_code* t2 = t1->next; // 暂时保存待删除的节点的地址
        t1->next = t1->next->next;          // 连接
        free(t2);                           // 删除节点
    }
    return ok;
}

char* zj_utf8_string::get_string() const
{

}

// 获取单个 UTF-8 字符，索引从 0 开始
char* zj_utf8_string::get_character(int32_t index) const
{

}

int32_t zj_utf8_string::get_character_utf8code (int32_t index) const
{

}

int32_t zj_utf8_string::get_character_utf16code(int32_t index) const
{

}