#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

// 定义向量和矩阵类型
using Vector = std::vector<double>;
using Matrix = std::vector<std::vector<double>>;

// 矩阵运算工具函数
Matrix matrixMultiply(const Matrix& A, const Matrix& B) {
    size_t m = A.size();
    size_t n = B[0].size();
    size_t p = B.size();
    Matrix C(m, std::vector<double>(n, 0.0));
    for (size_t  i = 0; i < m; ++i) {
        for (size_t  j = 0; j < n; ++j) {
            for (size_t  k = 0; k < p; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

Vector matrixVectorMultiply(const Matrix& A, const Vector& b) {
    size_t  m = A.size();
    size_t  n = A[0].size();
    Vector result(m, 0.0);
    for (size_t  i = 0; i < m; ++i) {
        for (size_t  j = 0; j < n; ++j) {
            result[i] += A[i][j] * b[j];
        }
    }
    return result;
}

Vector vectorAdd(const Vector& a, const Vector& b) {
    Vector result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

Vector vectorSubtract(const Vector& a, const Vector& b) {
    Vector result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

// 向量标量乘法
Vector vectorMultiply(double scalar, const Vector& v) {
    Vector result(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        result[i] = scalar * v[i];
    }
    return result;
}

// 矩阵加法
Matrix matrixAdd(const Matrix& A, const Matrix& B) {
    size_t  m = A.size();
    size_t  n = A[0].size();
    Matrix C(m, std::vector<double>(n, 0.0));
    for (size_t  i = 0; i < m; ++i) {
        for (size_t  j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// 矩阵减法
Matrix matrixSubtract(const Matrix& A, const Matrix& B) {
    size_t  m = A.size();
    size_t  n = A[0].size();
    Matrix C(m, std::vector<double>(n, 0.0));
    for (size_t  i = 0; i < m; ++i) {
        for (size_t  j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

// 矩阵转置
Matrix transpose(const Matrix& A) {
    size_t  m = A.size();
    size_t  n = A[0].size();
    Matrix B(n, std::vector<double>(m, 0.0));
    for (size_t  i = 0; i < m; ++i) {
        for (size_t  j = 0; j < n; ++j) {
            B[j][i] = A[i][j];
        }
    }
    return B;
}

// 矩阵标量乘法
Matrix matrixNormalMultiply(double scalar, const Matrix& A) {
	size_t  m = A.size();
	size_t  n = A[0].size();
	Matrix B(m, std::vector<double>(n, 0.0));
	for (size_t  i = 0; i < m; ++i) {
		for (size_t  j = 0; j < n; ++j) {
			B[i][j] = scalar * A[i][j];
		}
	}
	return B;
}

// 矩阵求逆（这里使用了简单的方法，实际应用中可能需要更复杂的算法）
Matrix matrixInverse(const Matrix& A) {
    size_t  n = A.size();
    Matrix inverse(n, std::vector<double>(n, 0.0));
    double determinant = 1.0;

    // 这里假设A是一个2x2矩阵
    if (n == 2) {
        double det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
        if (det == 0) {
            throw std::runtime_error("Matrix is singular");
        }
        inverse[0][0] = A[1][1] / det;
        inverse[0][1] = -A[0][1] / det;
        inverse[1][0] = -A[1][0] / det;
        inverse[1][1] = A[0][0] / det;
        return inverse;
    }
    else if (n == 1) {
		inverse[0][0] = 1 / A[0][0];
		return inverse;
    }
    else {
        throw std::runtime_error("Inverse not implemented for matrices larger than 2x2");
    }
}

// 生成立方点集
std::vector<Vector> generateCubaturePoints(size_t  n) {
    std::vector<Vector> points;
    for (size_t  i = 0; i < n; ++i) {
        Vector point(n, 0.0);
        point[i] = std::sqrt(n);
        points.push_back(point);
        point[i] = -std::sqrt(n);
        points.push_back(point);
    }
    return points;
}

// CKF 类
class CKF {
private:
    size_t  dim_x, dim_z; // 状态维度和观测维度
    Matrix F, Q, H, R; // 状态转移矩阵，过程噪声协方差，观测矩阵，观测噪声协方差
    Vector x; // 状态向量
    Matrix P; // 状态协方差矩阵

public:
    CKF(size_t  state_dim, size_t  meas_dim) : dim_x(state_dim), dim_z(meas_dim) {
        x = Vector(dim_x, 0.0);
        P = Matrix(dim_x, std::vector<double>(dim_x, 0.0));
    }

    void init(const Vector& x_init, const Matrix& P_init, const Matrix& F_init, const Matrix& Q_init, const Matrix& H_init, const Matrix& R_init) {
        x = x_init;
        P = P_init;
        F = F_init;
        Q = Q_init;
        H = H_init;
        R = R_init;
    }

    void predict() {
        // 生成立方点集
        auto points = generateCubaturePoints(dim_x);

        // 计算预测状态和协方差
        Vector x_pred(dim_x, 0.0);
        Matrix P_pred(dim_x, std::vector<double>(dim_x, 0.0));

        for (const auto& point: points) {
            Vector x_aug = vectorAdd(x, point);
            Vector x_prop = matrixVectorMultiply(F, x_aug); // 这里假设F是线性的
            x_pred = vectorAdd(x_pred, x_prop);
        }
        x_pred = vectorMultiply(1.0 / (2 * dim_x), x_pred);

        for (const auto& point: points) {
            Vector x_aug = vectorAdd(x, point);
            Vector x_prop = matrixVectorMultiply(F, x_aug);
            Vector diff = vectorSubtract(x_prop, x_pred);
            for (size_t  i = 0; i < dim_x; ++i) {
                for (size_t  j = 0; j < dim_x; ++j) {
                    P_pred[i][j] += diff[i] * diff[j];
                }
            }
        }
        P_pred = matrixNormalMultiply(1.0 / (2 * dim_x), P_pred);
        P_pred = matrixAdd(P_pred, Q); // 添加过程噪声

		// 更新状态和协方差
		x = x_pred;
		P = P_pred;
    }

    void update(const Vector& z) {
        // 生成立方点集
        auto points = generateCubaturePoints(dim_x);

        Vector z_pred(dim_z, 0.0);
        Matrix P_zz(dim_z, std::vector<double>(dim_z, 0.0));
        Matrix P_xz(dim_x, std::vector<double>(dim_z, 0.0));

        for (const auto& point : points) {
            Vector x_aug = vectorAdd(x, point);
            Vector z_prop = matrixVectorMultiply(H, x_aug); // 这里假设H是线性的
            z_pred = vectorAdd(z_pred, z_prop);
        }
        z_pred = vectorMultiply(1.0 / (2 * dim_x), z_pred);

        for (const auto& point: points) {
            Vector x_aug = vectorAdd(x, point);
            Vector z_prop = matrixVectorMultiply(H, x_aug);
            Vector diff_z = vectorSubtract(z_prop, z_pred);
            for (size_t  i = 0; i < dim_z; ++i) {
                for (size_t  j = 0; j < dim_z; ++j) {
                    P_zz[i][j] += diff_z[i] * diff_z[j];
                }
            }
            Vector diff_x = vectorSubtract(x_aug, x);
            for (size_t  i = 0; i < dim_x; ++i) {
                for (size_t  j = 0; j < dim_z; ++j) {
                    P_xz[i][j] += diff_x[i] * diff_z[j];
                }
            }
        }
        P_zz = matrixNormalMultiply(1.0 / (2 * dim_x), P_zz);
        P_xz = matrixNormalMultiply(1.0 / (2 * dim_x), P_xz);
        P_zz = matrixAdd(P_zz, R); // 添加观测噪声

        // 卡尔曼增益
        Matrix K = matrixMultiply(matrixMultiply(P_xz, matrixInverse(P_zz)), P_zz);

        // 更新状态和协方差
        Vector y = vectorSubtract(z, z_pred);
        x = vectorAdd(x, matrixVectorMultiply(K, y));
        P = matrixSubtract(P, matrixMultiply(matrixMultiply(K, P_zz), transpose(K)));
    }

    Vector getState() const { return x; }
    Matrix getCovariance() const { return P; }
};

int main() {
    // 初始化CKF
    CKF ckf(2, 1); // 2维状态，1维观测
    Vector x_init = { 0.0, 0.3 }; // 初始位置为0，速度为1
    Matrix P_init = { {1.0, 0.0}, {0.0, 1.0} };
    Matrix F = { {1.0, 1.0}, {0.0, 1.0} }; // 匀速行驶模型
    Matrix Q = { {0.01, 0.0}, {0.0, 0.01} }; // 减小过程噪声
    Matrix H = { {1.0, 0.0} }; // 观测矩阵
    Matrix R = { {0.1} }; // 减小观测噪声

    ckf.init(x_init, P_init, F, Q, H, R);

    // SFML 窗口设置
    sf::RenderWindow window(sf::VideoMode(2600, 600), "CKF Visualization");
    window.setFramerateLimit(60);

    // 定义一个圆形来表示位置
    sf::CircleShape truePosition(5);
    truePosition.setFillColor(sf::Color::Red);
    sf::CircleShape estimatedPosition(5);
    estimatedPosition.setFillColor(sf::Color::White);

    // 模拟数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);

    for (int t = 0; t < 100; ++t) {
        // 预测
        ckf.predict();

        // 生成观测值
        Vector z = { ckf.getState()[0] + d(gen) };

        // 更新
        ckf.update(z);

        // 绘制
        truePosition.setPosition(ckf.getState()[0] * 100, 300); // 缩放并居中
        estimatedPosition.setPosition(z[0] * 100, 300);

        window.clear();
        window.draw(truePosition);
        window.draw(estimatedPosition);
        window.display();

        // 控制绘制速度
        sf::sleep(sf::seconds(0.1));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}