# C++学习笔记

> 8月16日 static

## 静态

### 一个文件里的静态

相当于private，是私有的，无法被其他文件的extern找到

### 类里面的静态

首先要在一个文件里面定义过，然后才可以在类里面有static，不会报错

**效果**：类似于命名空间，访问使用class::variation(function)来访问

**注意**：静态函数只能访问静态变量，不可以访问非静态变量，这里面有点小复杂，看图即可

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-14-51-38-image.png)

### 局部静态

常用于函数内部的static，生存周期为程序周期，不会在函数结束之后就被释放，但是被限制在了某一块（函数）内部

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-15-00-48-image.png)

## 类与构造函数

如何去掉内部自带的默认构造函数？

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-15-30-10-image.png)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-15-30-30-image.png)

## 虚函数

子类可以重写父类中的虚函数的功能，覆写必须基类有虚函数，否则调用函数时先看基类

或者 加一个override (C++11引入)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-15-59-09-image.png)

但是有成本，要有多余的内存分配，嵌入式平台可以避免使用，其实也没多大影响吧

### 纯虚函数

令基类中虚函数为零，从此无法再实例化，只能借助子类实例化，子类要实现虚函数功能才可以创建示例

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-16-22-22-image.png)

使用：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-16-23-15-image.png)

## 可见性

protect可以在子类里被发现

可见性让代码更易维护

## 数组

使用指针在内存里跳来跳去也很影响性能，也许可以直接站上创建

**补充**：（c++11）std::array 内置数据结构  边界检查（当然也就可以获得数组的大小虽然有开销） 比原来的数组操作安全

栈中为数组申请内存，必须是一个在编译时就知道的常量

可以用static constexpr 或者直接static const 

## 字符串（c++）

字符串复制很慢，只读传参的时候可以常量引用``const std::string& string``

string实际上是模板类的实例化。加上了发送字符串到流中的重载。

加减符号也是重载。

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-18-59-56-image.png)

string实际就是`const char*`

### 字符串进阶：字符串表面量

``char* name  = "hello";`实际上是一个字符串常量，被嵌入系统是改不了的

，永远在系统的只读范围内，要改的话得用数组

宽字符和窄字符

```cpp
const wchar_t* name2 = L"Hello"; //
const char16_t* name3 = u"Hello";//utf16 双字节字符
const char32_t* name4 = U"Hello"; //utf32
```

c++14中有新的东西

string_literals

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-19-15-31-image.png)

后面的s是一种操作符

第二个：R--打印多行

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-19-18-00-image.png)

## const常量

`const int* a   `不能能改变指向的值

`int* const* a`不能改变指向

`类中`

```cpp
private:
int m_X, m_Y;
public:
int GetX() const  //只读模式，与变量无关
{

    // /！ m_X = 2 wrong!!
    return m_X;
}
```

```cpp
private:
int* m_X, m_Y;
public:
const int* const GetX() const  // 啥都别改
{

    //m_X = 2 wrong!!
    return m_X;
}
```

const 引用 

```cpp
void PrintEterity(const Class& c)
{
    //cant change 'c'
}
```

反抗const?就用mutable(可改变的)

## mutable关键字（lambda或const可能会见得到）

### const

const只能调用const

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-19-47-48-image.png)

好，现在我想调试，我要改变（大概也只有在这里用到了）：

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-19-50-38-image.png)

### lambda（一次性的小函数）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-19-54-21-image.png)

注意x的值还是8

## 构造函数初始化列表

成员初始化列表要注意按变量声明的顺序，分割用`,`号

**为什么要使用**

1. 代码看起来更简洁 

2. 提高性能建议使用，因为不用重复创建然后丢掉一个

## 三元操作（一个：一个？）

连着创建变量一起用

  三元数的嵌套，有点麻烦，有点疯狂

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-20-11-39-image.png)

## 浅谈c++初始化对象

先写一个易混的地方（**常量引用 + const函数**）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-20-19-07-image.png)

栈中创建变量，过了作用域就没了

用new创建的内存是在堆上的

在堆上创建变量可能更耗时，并且手动delete，可能会忘记

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-20-33-15-image.png)

补充：Enterty是自己创建的一个类

建议用智能指针

## new关键字

> 编写c++程序要注意的是内存，性能和优化问题，性能特别好

new很好用，是因为它不仅分配了内存还帮你构造了函数

malloc就不行

**切记切记**：要用delete

也有一些其他的：基于作用域的指针，引用计数等等

## 隐式转化

```cpp
class EAntity
{
    private:
std::string m_name;
int m_age;
public:
Entity(const std::string& name)
    :m_name(name),m_age(-1){}
Entiity(int age)
    :m_name("Unknown"),m_age(age){}
};

 void PrintEntity(const Entity& rntity)
{
    //printing
}

void main()
{
    PrintEntity(22); //正确
    PrintEntity(std::string("Hello"));//正确
    //PrintEntity("Hello")  //错误，只能隐式转化一次
    Entity e;
    Entity a = "Hello"; //隐式转化
    Entity b = 22;    
    //一般Entity(22);
}
```

禁用隐式

构造函数前加一个关键字 `explicit`

数学库，或者低级封装

## c++运算符重载

运算符是代替函数的东西

可以运算符中调用操作函数，也可以是操作函数中调用运算符

左移运算符重载

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-21-53-42-image.png)

## 类中的this

this会随着操作函数有没有加const来自己调整有没有加const

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-22-20-04-image.png)

当然在类里面用类外的东西传参就可以用this

`Eternity& e = *this;`

甚至可以`delete this;`

## 栈--一种数据结构

> 栈和堆可以了解的东西很多，后面慢慢看

创建一个作用域指针 mutex_locking（可以品一品）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-22-48-39-image.png)

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-16-22-49-00-image.png)

ScopePtr内存是在栈上面分配的，所以会被释放，即使用new创建

## 智能指针

### unique pointer(作用域指针)

`include <memory>`

**注意**：智能指针不支持拷贝（会报错）

翻看定义的时候拷贝构造函数和拷贝构造操作符都被删掉了

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-09-08-32-image.png)

### shared_prt

通过引用计数，引用计数为0，释放（引用计数系统）

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-09-13-12-image.png)

### weak pointer 做和shared pointer一样的事，不增加引用开销

退出里面的作用域的时候，e0就被释放了

![](C:\Users\Delig\AppData\Roaming\marktext\images\2024-08-17-09-17-35-image.png)


