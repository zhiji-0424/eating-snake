#include "zj_double_linkedlist.hpp"

#define LOGE(...) zj_logger_e("zj_double_linkedlist", __VA_ARGS__);
#if defined(ZJ_DEBUG_ALL) || defined(ZJ_DOUBLE_LINKEDLIST_HPP)
#  define LOGD(...) zj_logger_d("zj_double_linkedlist", __VA_ARGS__)
#else
#  define LOGD(...) (void)0
#endif

