#if !defined(ZJ_DOUBLE_LINKEDLIST_HPP)
#define ZJ_DOUBLE_LINKEDLIST_HPP

#include <zj_logger.h>

template <typename Type>
class zj_double_linkedlist {
    public:
        zj_double_linkedlist();
        ~zj_double_linkedlist();

        bool insert(int32_t index, const Type& e);  //在 index 之前插入数据
        bool append(const Type& e);                 //在链表后面增加数据
        bool del(int32_t index);                    //删除索引为 index 的节点
        bool set(int32_t index, const Type& e);     //修改索引为 index 的节点的内容
        Type& at(int32_t index);                    //返回在 index 上的数据

        Type& operator[] (int32_t index) { return at(index); };

    private:
        int32_t length;     //长度
        int32_t last_index; //上一次的位置
        void*   first;      //第一个节点
        void*   last_node;  //上一次访问的节点
};

#endif //ZJ_DOUBLE_LINKEDLIST_HPP