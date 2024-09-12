# c++学习第五天

> 8月22号

## 最小字符串

字符串要用的号，（也许减少字符串的使用会更快）

不需要堆分配，可以自行分配，（似乎是小于15个字符就可以在栈上分配，可以去定义里面找一找，也可以重载new运算符自己看）

`std::string name = "Cherno";`看似在堆上分配内存但实际上没有，cpp内部优化了

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-21-11-54-57-image.png)

当然是在release模式下才好

## 跟踪系统内存

重载new，然后插入一个断点

重载delete，可以打印出你想看到的内容

甚至可以写一个内存检测器：AllocationMetrics(class创建的一个单例或实例)

## 左值和右值

移动语义

左值引用和右值引用

左值引用

`int  i = 10;//`

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-21-12-15-41-image.png)

即使你加了一个const表示与右值兼容但还是会有限选择下面的右值重载

右值是一个临时值，左值是有存储变量功能的

## 持续集成（CI）

多平台，多配置

自动化测试，每次提交到github上面的时候就会自动构建和测试应用

用`jenkins`并希望一直跑

## 静态分析

检查源代码，查bug的

有点像代码复查

但估计也需要钱

## C++参数计算顺序

argument evaluation order

正确地答案是未定义的行为，实际取决于编译器

[C++ gcc 13.2.0 - Wandbox](https://wandbox.org/)



## 移动语义

左值引用和右值引用是基础

适用于在堆分配内存（字符串）影响性能的

本质上的操作就是指针，浅复制，将一个对象移到另一个

##### std::move

将一个字符串站转化成临时的：(String&&)强制转化或者std::move优雅地转化






