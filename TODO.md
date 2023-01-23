发布流程：
    Native层: (1)添加v7a v8a x86 x86_64, (2)设置不调试
    Java层：  (1)debug_mode = false; (2)编译
    复制 libs/(两个ABI文件夹)
    [www.pgyer.com/app/publish]


适配 Android 5~13
(android:exported="true")

arm-v7a-neon

Lua脚本编辑-运行

设计UI库(或者寻找)
android文件选择器
#文本输入(选择、光标移动)
draw画文本(内置链表，不导出)
画点
画圆BUG: 不适应glViewport()分割，试用极限法画圆

钟表(计时器、倒计时、表盘、每隔一段时间提醒)、词典(自己的)

utf8转换(错误判断)
1. 0000 - 007f           | 0xxx xxxx
2. 0080 - 07ff           | 110x xxxx  10xx xxxx
3. 0800 - ffff           | 1110 xxxx  10xx xxxx  10xx xxxx
4. 0001 0000 - 0010 ffff | 1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx
5. 0020 0000 - 03ff ffff | 1111 10xx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx
6. 0400 0000 - 7fff ffff | 1111 110x  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx
[最多6字节；验证表达的字节数与实际字节数；除第一个外的字节都以10开头，单独出现就忽略]
[增加函数：c_str字符串 ==> u16c数组，并在zj_string中集成]

使用单Activity模式，调试时发生错误直接log_cat

重新界面设计

坐标系转换--平面直角坐标系

crash界面上传到 蒲公英

关于(about)界面：
    图标设计：墨刀
    参与测评：

返回按钮

初始长度:3

死亡时震动

分数记录

方块间隙
自动连接相邻方块

规则说明：
    移动、死亡、开始暂停

墙用方块来表现

增加难度：随机出现方块

屏幕满了？下一关

显示更新内容

自定义方块数量大小
