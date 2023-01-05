发布流程：
    Native层: (1)添加armeabi-v7a, (2)all_NDEBUG
    Java层：  (1)debug_mode = false; (2)项目分支releaase (3)项目刷新 (4)编译
    复制 libs/(两个ABI文件夹)
    浏览器[www.pgyer.com/app/publish]

适配 Android 12 (android:exported="true")

3D:使用bgfx
2D:参考solar2D

Android.mk --> 把 绘图API 单独编译成库
设计UI库
android文件选择器
draw画文本(内置链表，不导出)
画点


悬浮便签、相册（自定义扫描目录）、钟表(计时器、倒计时、表盘、每隔一段时间提醒)、词典(自己的、还有翻译API的)

gitee更新 绘图API、贪吃蛇



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
