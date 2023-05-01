发布流程：
    Native层: (1)添加v7a v8a x86 x86_64, (2)设置不调试
    Java层：  (1)debug_mode = false; (2)编译
    复制 libs/(两个ABI文件夹)
    [www.pgyer.com/app/publish]


适配 Android 5~13
(android:exported="true"
android:debuggable)




1. string-utf8

防返回键误触
限制显示区域
字体缩放(使用DPI)





Lua脚本编辑-运行

android文件选择器
#文本输入(选择、光标移动)
draw画文本(内置链表，不导出)

时钟(计时器、倒计时、表盘、每隔一段时间提醒)、词典(自己的)

zj_utf8_string
[最多7字节；验证表达的字节数与实际字节数；除第一个外的字节都以10开头，单独出现就忽略]
[增加函数：c_str字符串 ==> u16c数组，并在zj_string中集成]
[与zj_string互通]

检测:
    10xxx
    0xxxx
    header byte

链表储存

type:
    zj_utf8_code:
        pointer: next
        pointer: data  //['ex' 'es' 'er' rd' ...]
        int:     num   //data size

    zj_utf8_string:
        pointer: first
        int:     length