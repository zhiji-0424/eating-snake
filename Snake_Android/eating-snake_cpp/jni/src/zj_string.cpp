#include "zj_string.hpp"

#define LOGE(...) zj_logger_e("zj_string", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DEBUG_STRING)
#  define LOGD(...) zj_logger_d("zj_string", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

static char* malloc_text(size_t size)
{
    if (size == 0) {
        LOGE("malloc_text: wanted to alloc 0 byte memory!");
        return nullptr;
    }
    size += 1; // The end is '\0'.
    LOGD("malloc_text: real size:%ld", size);
    char* res = (char*)malloc(size);
    if (res == nullptr) {
        LOGE("malloc_text: out of memory! request size: %ld", size);
        return nullptr;
    }
    LOGD("malloc_text: res:%p", res);
    memset(res, '\0', size);
    return res;
}

zj_string::zj_string()
{
    LOGD("zj_string()");
    data = nullptr;
    length = 0;
}

zj_string::zj_string(char ch)
{
    LOGD("zj_string(char):(%d)%c", ch, ch);
    if (ch != '\0') {
        data = malloc_text(1);
        if (data != nullptr) {
            data[0] = ch;
            length  = 1;
            LOGD("zj_string(char): data:(%p)%s, length:%d", data, data, length);
        } else {
            LOGE("zj_string(char): Fail to alloc memories!");
        }
    } else {
        LOGE("zj_string(char): ch is '\\0'");
    }
    data    = nullptr;
    length  = 0;
}

zj_string::zj_string(const char* str)
{
    LOGD("zj_string(const char*): str:(%p)%s", str, str);
    create(str);
    LOGD("zj_string(const char*): data:(%p)%s, length:%d", data, data, length);
}

zj_string::zj_string(const zj_string& str)
{
    LOGD("zj_string(const zj_string&): str.c_str():(%p)%s", str.c_str(), str.c_str());
    create(str.c_str());
    LOGD("zj_string(const zj_string&): data:(%p)%s, length:%d", data, data, length);
}

void zj_string::create(const char* str)
{
    LOGD("create: str:(%p)%s", str, str);
    if (str != nullptr) {
        if (str[0] != '\0') {
            size_t len = strlen(str);
            data = malloc_text(len);
            if (data != nullptr) {
                strcpy(data, str);
                length = len;
                return;
            } else {
                LOGE("create: Fail to alloc memories!");
            }
        } else {
            LOGE("create: str[0] is '\\0'. str's address:(%p)", str)
        }
    } else {
        LOGE("create: str is null.");
    }
    data = nullptr;
    length = 0;
}

zj_string::~zj_string()
{
    LOGD("~zj_string()");
    this->destroy();
}

void zj_string::destroy()
{
    LOGD("destroy: data:(%p)%s, length:%ld", data, data, length);
    if (data != nullptr) {
        free(data);
        data = nullptr;
    }
    length = 0;
}

zj_string& zj_string::operator=(const zj_string& str)
{
    if (this == &str) {
        LOGD("operator=: The same string.");
    } else {
        LOGD("operator=: destroy last data");
        this->destroy();  // 很重要
        LOGD("operator=: str.c_str():(%p)%s", str.c_str(), str.c_str());
        this->create(str.c_str());
        LOGD("operator=: data:(%p)%s, length:%ld", data, data, length);
    }
    return *this;
}

zj_string zj_string::operator+(const zj_string& str2)
{
    LOGD("operator+");
    zj_string str;
    const char* t1 = this->c_str();
    const char* t2 = str2.c_str();
    if (t1 == nullptr) {
        LOGD("operator+: case 1: str1.c_str() == nullptr");
        LOGD("operator+: str2.c_str():(%p)%s", t2, t2);
        str.create(t2);
        LOGD("operator+: str.data:(%p)%s", str.c_str(), str.c_str());
    } else if (t2 == nullptr) {
        LOGD("operator+: case 2: str2.c_str() == nullptr");
        LOGD("operator+: str1.c_str():(%p)%s", t1, t1);
        str.create(t1);
        LOGD("operator+: str.data:(%p)%s", str.c_str(), str.c_str());
    } else {
        LOGD("operator+: case 3: normally");
        LOGD("operator+: str1.c_str():(%p)%s, str2.c_str():(%p)%s", t1, t1, t2, t2);
        size_t len = this->size() + str2.size();
        char* tmp = malloc_text(len);   // 拼起来传给create()
        if (tmp != nullptr) {
            strcpy(tmp, t1);
            strcat(tmp, t2);
            LOGD("operator+: tmp:(%p)%s, len:%ld", tmp, tmp, len);
            str.create(tmp);
            LOGD("operator+: str.data:(%p)%s", str.c_str(), str.c_str());
            free(tmp);
        } else {
            LOGE("operator+: Fail to alloc memories!");
        }
    }
    return str;
}

zj_string& zj_string::operator+=(const zj_string& str)
{
    LOGD("operator+=: str.c_str():(%p)%s", str.c_str(), str.c_str());
    return *this = *this + str;
}

bool zj_string::operator==(const zj_string& str2)
{
    if (this == &str2) {
        return true;
    }
    if (this->size() == str2.size()) {
        const char* t1 = this->c_str();
        const char* t2 = str2.c_str();
        LOGD("operator==: str1.c_str():(%p)%s, str2.c_str():(%p)%s", t1, t1, t2, t2);
        if (t1 == nullptr /*|| !t2 */ || (strcmp(t1, t2) == 0)) {
            // 当长度相等时, 若t1==null, 则t1==t2==null
            return true;
        }
    }
    LOGD("oerator==: size() are not equal.");
    return false;
}
