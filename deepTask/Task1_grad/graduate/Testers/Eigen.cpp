#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main() {
    // 定义两个2x2矩阵
    Matrix2d mat1;
    Matrix2d mat2;

    // 初始化矩阵
    mat1 << 1, 2,
        3, 4;
    mat2 << 2, 3,
        1, 4;

    // 矩阵相加
    Matrix2d mat_sum = mat1 + mat2;

    // 矩阵相乘
    Matrix2d mat_product = mat1 * mat2;

    // 输出结果
    cout << "Matrix 1:\n" << mat1 << endl;
    cout << "Matrix 2:\n" << mat2 << endl;
    cout << "Sum of matrices:\n" << mat_sum << endl;
    cout << "Product of matrices:\n" << mat_product << endl;

    return 0;
}
