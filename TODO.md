发布流程：
    Native层: (1)添加armeabi-v7a, (2)all_NDEBUG
    Java层：  (1)debug_mode = false; (2)项目分支releaase (3)项目刷新 (4)编译
    复制 libs/(两个ABI文件夹)
    VIA浏览器[www.pgyer.com/app/publish]

适配 Android 12 (android:exported="true")

3D:使用bgfx

Android.mk --> 把 绘图API 单独编译成库
设计UI库
android文件选择器
文本输入(选择、光标移动)
draw画文本(内置链表，不导出)
画点
画圆BUG: 不适应glViewport()分割，试用极限法画圆

悬浮便签、相册（自定义扫描目录）、钟表(计时器、倒计时、表盘、每隔一段时间提醒)、词典(自己的、还有翻译API的)

gitee更新 绘图API、贪吃蛇

utf8转换(错误判断)
1. 0000 - 007f           | 0xxx xxxx
2. 0080 - 07ff           | 110x xxxx  10xx xxxx
3. 0800 - ffff           | 1110 xxxx  10xx xxxx  10xx xxxx
4. 0001 0000 - 0010 ffff | 1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx
5. 0020 0000 - 03ff ffff | 1111 10xx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx
6. 0400 0000 - 7fff ffff | 1111 110x  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx
[最多6字节；验证表达的字节数与实际字节数；除第一个外的字节都以10开头，单独出现就忽略]
[增加函数：c_str字符串 ==> u16c数组，并在zj_string中集成]


界面设计

坐标系转换--平面直角坐标系

crash界面上传到 蒲公英

json/database
登录界面
输入用户名字(记录分数)

关于界面：
    图标设计：墨刀
    参与测评：

死亡时震动

分数记录

方块间隙
自动连接相邻方块

规则说明：
    移动、死亡、开始暂停

墙用同种方块

增加难度：随机方块

屏幕满了？下一关


