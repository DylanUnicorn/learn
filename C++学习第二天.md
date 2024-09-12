# C++学习第二天

> 8月17日

## c++的复制和拷贝构造函数

1. 默认构造函数都是浅复制，就是复制指针的地址
   
   ![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-09-43-35-image.png)
   
   或者：
   
   ![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-09-44-19-image.png)

2. 所以需要自己写深复制
   
   ![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-09-46-28-image.png)

3. 基础使用的时候，多用const引用（字符串），但是某些时候可能会更块

## 重载箭头运算符

### 重载

```cpp
class Entity
{
    public:
int x;
public:
    void Print() const {std::cout << "Hello!" <<  std::endl;}
};

class ScopedPtr
{
    Private:
    Entity* m_obj;
public:
    ScopedPtr(Entity* entity)
        : m_obj(entity)
    {
    }

    ~ScopedPtr()
{
    delete m_obj;//谨慎删除俩次
} 
    Entity* operater->() const
{
    return m_Obj;
}
    const Entity* operater->() const //const版本
{
    return m_obj;
}
} 
```

main函数中：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-10-12-43-image.png)

或者

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-10-12-17-image.png)

都可以

**看起来跟简洁了**

### 获取内存偏移量

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-10-16-54-image.png)

## 动态数组

`include <vector>`(实际上是模板)

### 基本用法

参考代码：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-10-35-51-image.png)

引用传参很好用，vertiice有x,x,z

### 优化，压榨性能，跑的更快

老vector：增加，复制，删除

**避免复制**（可以通过创建复制构造函数观察）

1. 告诉vector自己可能要放多少个，防止自己复制扩张

2. 将对象直接在vector上面创建，而不是在main函数上面创建再push_back

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-10-50-23-image.png)

 这样基本上就不会复制那么多遍了

## 静态链接（使用外部库，二进制链接）

区别：静态链接是库被放到可执行文件中(.exe)

动态： 运行时被链接的，可以选择在程序运行时链接（.dll）

.exe要和.dll一起

静态链接理论上可以产生更快的应用程序。

vs如何设置懒得改了，忘记保存

[动态链接](https://www.bilibili.com/video/BV1oD4y1h7S3?p=50)

## 栈和堆的比较

ram实际存在的俩区域

物理位置是一样的，

#### 内存分配的区别

`int value =5`

`int* hvalue = new int`

 栈分配内存实际就是移动了栈的指针,结束时回到原来的位置，释放,本质就是一条pu命令

new是调用一个malloc函数，调用底层函数，堆上分配内存（通过空闲列表来寻找），然后给指针，记录被拿走了多少内存

缓存优化可以了解一下：cashe miss(游戏引擎应用)

## auto

  有可能造成破坏，影响原本需要依赖的代码

循环：for each ，基于range的循环，迭代器循环

**应用场景**：

1. 类型名很长，比如vector中的迭代器

`std::vector<std::string>::std::iterator it`就可以换成`auto it   `

2. 像这种（`include <unordered_map>`）；
   
   ![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-18-06-13-image.png)
   
   可以用using或者typedef
   
   ![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-18-08-06-image.png)
   
   或者auto,但是该引用还是要引用，该const还是要const

c++11可以使用后置返回类型

## 静态数组(不增长的数组)

`include <array>`

和C语言的数组很像，但是可以访问数组大小data.size()

vector存贮在堆中

std::array 存贮在栈中, 可以bounce checking 打开源代码可见，而且没有性能成本

## 来自C语言的函数指针

将一个函数赋值给一个变量

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-19-54-05-image.png)

加一个int![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-19-55-02-image.png)

看个有趣的，传参传入匿名函数lambda

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-20-03-20-image.png)

## c++lambda(匿名函数)

用完即销

一般只要用函数指针，都可以用lambda

推荐网站：[Lambda](https://en.cppreference.com/)

参考示例代码（迭代器获取符合条件的值，*it=5）：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-20-21-18-image.png)

## 用std：：大概就是给自己看看自己应用了什么库

## 严肃的项目：代码写namespaced的后面

## C++线程（并行）

`include<thread>`

std::thread创建一个线程（变量）

通过构造函数传递一个函数（开启）

持续直至完成（xx.join）



**延时**

```cpp
using namespace std::literals::chrono_literals;

std::this_thread::sleep_for(1s)
```

**获得id**

`std::this_thread::get_id()`



**之前研究过***但是忘记了



## 计时

性能评估，基准测试，代码跑的多块

1. chrono

2. QuerypPerformerceCounter

用计时看到差异

```cpp
#include <chrono>

int main(){
using namespacestd::literals::chrono::chrono_literals

auto start = std::chrono::high_resolution_clock::now()
std::this_thread::sleep_for(1s);
auto end = std::chrono::high_resolution_clock:now();

std::chrono::duration<float> duration = end - start;


std::cout<< duration.count()<< "s" << std::enld;


}
```

自动计时

```cpp
struct Timer
{
    std::chrono::time_point<std::choros::steady_clock> start,end;
    std::chrono::duration<float> duration;
 

    Timer()
{
    start  = std::chrono::high_resolution_clock::now();  
}
    ~Timer()
{
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    float ms = duration.count() * 1000.0f; //毫秒
    std::cout<< "Time took" << ms<<"ms"<<std::endl;    
}
}


void function()
{
    Timer timer;
    
    for(int i=0;i<100;i++)
{
    std::cout<< "hello"<< std::endl;
    //std::cout<<"hello\n";可能会更块
}
}
```

基准测试还没讲

intrumentation

游戏开发，游戏引擎


