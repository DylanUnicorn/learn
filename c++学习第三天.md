# c++学习第三天

## c++多维数组

内存碎片，一维数组是连续的。

但是创建指针，然后在指针里面创建数组会造成内存碎片，花费时间在ram找数据

所以基本都可在一维数组中解决，这样显然会快一点

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-08-55-15-image.png)

即使是位图也可以用一维数组解决

## 排序(sorting)

`std::sort`

[sort]([std::sort - cppreference.com](https://en.cppreference.com/w/cpp/algorithm/sort))

## 类型双关

将类型互相转化（同一个内存而不是新建一个）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-09-15-49-image.png)

虽然这样会读取不属于自己的内存

可以看下方的操作：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-09-20-56-image.png)

`int y = ...`的部分是一个疯狂的操作，内存操作了

#### 联合体

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-09-35-50-image.png)

## 虚析构函数

处理多态很好用

基类的指针接受子类开辟的空间，有可能子类的析构函数无法被调用

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-09-47-10-image.png)

造成内存泄漏

## casting（类型转化）

#### 显式类型转化

c风格

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-09-51-29-image.png)

cpp风格（排错比较方便）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-18-09-52-19-image.png)

尝试构建

dynamic_cast, reinterpret_cast(C风格默认), static_cast ,

const_cast

## 条件操作断点

右键断点，然后假如condition,或者action

比如在一个loop,特定的值触发断点

虽然断点会变慢你的应用程序

## 预编译头文件

大大节省编译时间（适合一些不经常改变的头文件比如log）

[头文件预编译]([【72】C++的预编译头文件_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1oD4y1h7S3?p=72&vd_source=9cb032883db25037abe8cafd01158325))

vs 的设置有点麻烦
