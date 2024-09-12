# 类型推导之deltype表达式

### 一言蔽之：就是可以自动推导类型，只需要给一个变量或者函数或者成员函数即可

### 复杂情况：

1. 函数
   
   ```cpp
   int foo();
   decltype(foo()) result = foo(); // decltype推导出foo()函数的返回类型
   
   ```

2. 泛型编程
   
   ```cpp
   template<typename T,typename U>
   auto add(T t,U u) -> deltype(t + u )
   {
       return r + u; //自动推导出r+u的类型
   }
   ```

3. 表达式模板
   
   ```cpp
   template<typename T>
   T process(T value);
   
   decltype(process<int>(42)) processedValue = process<int>(42); // decltype根据模板实例化推导出返回类型
   
   ```

4. l临时成员
   
   ```cpp
   struct Point {
       double x;
       double y;
   };
   
   Point p;
   decltype(p.x) px = p.x; // decltype推导出p.x的类型
   decltype(*&px) ref = px; // decltype推导出px的引用类型
   decltype(std::declval<Point>().y) py = 5.0; // decltype推导出临时对象Point().y的类型
   
   ```



## 工程运用

```cpp
template<typename T, typename U>
auto multiply(T t, U u) -> decltype(t * u) {
    return t * u;
}

int main() {
    int x = 10;
    double y = 5.2;
    auto result = multiply(x, y); // 结果的类型根据x * y的结果自动为double
    return 0;
}

```

自动推导特别方便

.........和auto功能类似只不过这个可用范围更广一些



## declval

一般和deltype喝着用，一个用于接受不经过构造函数使用的成员函数的返回值，一个推到返回值的类型

**官方代码**

```cpp
#include <iostream>
#include <utility>
 
struct Default
{
    int foo() const { return 1; }
};
 
struct NonDefault
{
    NonDefault() = delete;
    int foo() const { return 1; }
};
 
int main()
{
    decltype(Default().foo()) n1 = 1;                   // n1 的类型是 int
//  decltype(NonDefault().foo()) n2 = n1;               // 错误：无默认构造函数
    decltype(std::declval<NonDefault>().foo()) n2 = n1; // n2 的类型是 int
    std::cout << "n1 = " << n1 << '\n'
              << "n2 = " << n2 << '\n';
}
```

输出：

```cpp
n1 = 1
n2 = 1
```



## 接下来可以讲概念库了（concept）

`#include <concept>`

一言蔽之：指定**模板参数必**须满足的条件，在编译时就被捕捉

类别众多，需要用的时候自己查

### 基本用法

```cpp
#include <concepts>

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>; // 要求 T 类型支持 + 运算符，且返回类型是 T
};

template <Addable T>
T add(T a, T b) {
    return a + b;
}

int main() {
    int x = 1, y = 2;
    auto result = add(x, y); // 正常使用
    // auto fail = add("a", "b"); // 编译错误：std::string 不满足 Addable
}


```

### 常用类别

1. 约束
   
   - `std::integral`：限制为整数类型。
   - `std::floating_point`：限制为浮点类型。
   - `std::convertible_to<U>`：要求能转换为类型 `U`。

2. 复合概念
   
   ```cpp
   template <typename T>
   concept Number = std::integral<T> || std::floating_point<T>;
   ```

3. 模板参数约束
   
   ```cpp
   template <std::integral T>
   T multiply(T a, T b) {
       return a * b;
   }
   
   ```

## 工程运用

限制排序

限制传入数学运算的数据，搭建数学库

矩阵类：

```cpp
#include <iostream>
#include <vector>
#include <concepts>

// 定义一个概念，要求类型T支持加法操作
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// 定义一个概念，要求类型T支持乘法操作
template<typename T>
concept Multipliable = requires(T a, T b) {
    { a * b } -> std::same_as<T>;
};

// 定义一个概念，要求类型T是一个数值类型
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// 矩阵类
template<Numeric T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows, cols;

public:
    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<T>(c)) {}

    // 矩阵加法
    Matrix operator+(const Matrix& other) const requires Addable<T> {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match for addition.");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    // 矩阵乘法
    Matrix operator*(const Matrix& other) const requires Multipliable<T> {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions must match for multiplication.");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // 访问元素
    T& operator()(size_t r, size_t c) {
        return data[r][c];
    }

    const T& operator()(size_t r, size_t c) const {
        return data[r][c];
    }

    // 打印矩阵
    void print() const {
        for (const auto& row : data) {
            for (const auto& elem : row) {
                std::cout << elem << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    Matrix<int> mat1(2, 3);
    Matrix<int> mat2(2, 3);

    // 初始化矩阵
    mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
    mat1(1, 0) = 4; mat1(1, 1) = 5; mat1(1, 2) = 6;

    mat2(0, 0) = 7; mat2(0, 1) = 8; mat2(0, 2) = 9;
    mat2(1, 0) = 10; mat2(1, 1) = 11; mat2(1, 2) = 12;

    // 矩阵加法
    auto matSum = mat1 + mat2;
    std::cout << "Matrix Addition:\n";
    matSum.print();

    // 矩阵乘法
    Matrix<int> mat3(3, 2);
    mat3(0, 0) = 1; mat3(0, 1) = 2;
    mat3(1, 0) = 3; mat3(1, 1) = 4;
    mat3(2, 0) = 5; mat3(2, 1) = 6;

    auto matProd = mat1 * mat3;
    std::cout << "Matrix Multiplication:\n";
    matProd.print();

    return 0;
}

```


