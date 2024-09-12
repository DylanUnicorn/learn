# C++学习第四天

> 8月19日

## dynamic casting类型转化

专门用于沿继承结构的类型转化

子类转成基类或基类转成派生类

可以检查返回是否为NULL

```cpp
class Entity
{
public:
    virtual void PrintName(){}    
};

class Player : Entity
{

};
class Enermy : Enterty
{

};


int main()
{
    Player* player = new Player();
    Entity* actualluEnermy = Enermy();
    Enttity* actualPlayer = player;

    //Player* p = (Player*)e1; //这个有点危险,有可能包含enermy没有的player、


    //可以替换
   Player* p = dynamic_cast<Player*>(actaualllyEnermy); //需要一个多态类型  
    Player* p = dynamic_cast<Player*>(autuallyPlayer);
}
```

 打断点可以发现有一块是转换失败了，是无效的

#### 如何工作的

它存储了运行时的类型信息，是增加开销的（rtti），还需要时间检查信息是否匹配，

如果把vs的c++ -> language-> runtime_Type关掉就报错了

好处是可以验证：

`if(p0){}`

## 基准测试

测试一下跑的快不快

先写一段测试代码循环一万次

```cpp
#inlude <chrono>

int main()
{
    Timer timer();
    int value = 0;
    for(int i=0;i<1000000;i++)

{
    std::cout<< value<<std::endl;
}
```

计时器

```cpp
class Timer()
{
public:
    Timer(){
    m_startTImrpoint = std::chrono::high_resolution_clock::now();
}
    ~Timer(){
    Stop();
}
    void Stop(){
    auto endTimepoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_sice_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_sice_epoch().count();

    auto duration = end- start;
    double ms = duration * 0.001;

    std::cout<< duration << "us (" std<< ms << " ms)"<<std::endl;
}
}
```

**注意**：
编译器会积极的修改你的代码，请确保你去认识在测量那件事的耗时

可以在release条件下测量性能

现在测智能指针的性能

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-10-44-19-image.png)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-10-44-34-image.png)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-10-45-26-image.png)

结果make_shared > new_share > unique_ptr

## 插播一些广告

调研选题，比如农业，就要知道地形，播种作物，地形特性（难点）

问题分析

产品介绍

创新点

免耕法

口气播种

小型风压选种机

数值分析

作品，结果总览

实现难点：开环，自动化，特定环境，比智能控制更难

## 结构化绑定（处理返回值）

##### 以往做法

  c++赠送的：tuple，pares，std::tie

`include<tuple>`

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-10-56-35-image.png)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-10-56-51-image.png)

不是很好用，不如结构体

##### 结构化绑定（c++17）

一行代码搞定

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-10-59-03-image.png)

propety -> c++ -> language -> c++ language standard

## 处理optional数据（可能存在也可能不在，相当于内置bool）

文件处理

`include <optional>`

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-16-01-23-image.png)

假如文件中找不到，你可以使用你的默认设置（value_or）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-16-03-37-image.png)

## 单一变量存放多种类型的数据（c++17）

不用担心可能改变的数据（或者不知道接受了什么数据）

`include <variant>`

类型处理很安全，相当于创建了一个**结构体**

类型安全的union

union更有效率更好但是variant更安全，除非是做底层优化的。桌面平台建议多用

只需要看几个：

1. ![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-16-23-32-image.png)

2. `dara.index()`

3. get_if处理异常![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-16-26-37-image.png)

4. 改进optional，更详细的报错误![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-16-28-05-image.png)

## 存储任意类型的数据

可以用void指针来做，但可以用一个更新的方法

`#include<any>`

```cpp
std::any data;
data = 1;
data = "Hello";
data = std::string("Hello");
```

std::any data;

存储之后想拿就拿（`std::any_cast<std::string>(data)`）

只不过variant要求写出你的所有类型

```cpp
std::variant data
...
std::string string = std::get<std::string(data)>
```

但variant正好让你的代码更安全了

要知道any的原理可以看一下头文件

std::any 在处理较大的数据的时候会动态分配

防止复制：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-19-16-55-25-image.png)

注意右边的引用

或者：`std::string& string = std::get<std::string>(data);`

**总结**：不如variant（variant不会动态分配性能较好）

## 如何让c++跑的更快（多线程）

把一个for循环改成并行的for循环（c++有点难实现）

`#incldue<feature>`

1. mutex互斥锁

2. async

```cpp
for（const auto& flie : mashFilepath）

{    
    m_future.push_back(std::async(std::Launch::async, LoadMash,)m_Mashes, file))

}
```

s涉及东西很多，讲不明白

[【79】如何让C++运行得更快_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1oD4y1h7S3?p=79&vd_source=9cb032883db25037abe8cafd01158325)

并行堆栈查看：

debug  -> Windows ->parallel stacks

## 让字符串更快

std::string_view,指向内存的指针加上size

实际就是指针操作

c++17引用了一个类但是我们早就在使用了

几乎就是C语言中的字符串操作了

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-16-38-09-image.png)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-16-37-55-image.png)

假如上面的函数改了形参

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-16-39-48-image.png)

就会多一次分配

下面这个有四次分配：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-16-43-43-image.png)

**对性能有要求的还有注意一下**

## 可视化基准测试

([Basic Instrumentation Profiler · GitHub](https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e))

只需要一个网址：

Chrome://tracing

## 单例

面向对象编程的好处是可以多次实例化他们

当我不需要类的时候，就只需要单一对象的实例了

java典型所有东西都必须是一个类，同样也是在类中创建成员

比如随机数生成数，渲染器，

组织一堆全局变量和静态的东西

**注意**:构造函数不能public

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-18-45-09-image.png)

删除构造函数是为了只有一个单例

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-18-49-33-image.png)

或者把实例化静态放在个get函数里面：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-18-50-40-image.png)

main函数：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-18-51-51-image.png)

或者直接用命名空间

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-20-18-53-11-image.png)

emmm把main函数中的Random改成RandomClass即可

# 
