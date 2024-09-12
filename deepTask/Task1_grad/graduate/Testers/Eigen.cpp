#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main() {
    // ��������2x2����
    Matrix2d mat1;
    Matrix2d mat2;

    // ��ʼ������
    mat1 << 1, 2,
        3, 4;
    mat2 << 2, 3,
        1, 4;

    // �������
    Matrix2d mat_sum = mat1 + mat2;

    // �������
    Matrix2d mat_product = mat1 * mat2;

    // ������
    cout << "Matrix 1:\n" << mat1 << endl;
    cout << "Matrix 2:\n" << mat2 << endl;
    cout << "Sum of matrices:\n" << mat_sum << endl;
    cout << "Product of matrices:\n" << mat_product << endl;

    return 0;
}
