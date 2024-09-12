#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 这里编写一个线性回归模型
double predict(double x, double w, double b)
{
    return w * x + b;
}

// 计算损失函数
double loss(const vector<double>& x, const vector<double>& y, double w, double b)
{
    double sum_loss = 0;
    for (int i = 0; i < x.size(); i++) {
        double y_pre = x[i] * w + b;
        sum_loss += (y_pre - y[i]) * (y_pre - y[i]);
    }
    return sum_loss / x.size();
}

// 计算梯度
double dw(const vector<double>& x, const vector<double>& y, const double& w, const double& b)
{
    double sum_w = 0;
    for (int i = 0; i < x.size(); i++) {
        double y_pre = predict(x[i], w, b);
        sum_w += (y_pre - y[i]) * x[i];
    }
    return 2 * sum_w / x.size();
}

double db(const vector<double>& x, const vector<double>& y, const double& w, const double& b)
{
    double sum_b = 0;
    for (int i = 0; i < x.size(); i++) {
        double y_pre = predict(x[i], w, b);
        sum_b += (y_pre - y[i]);
    }
    return 2 * sum_b / x.size();
}

int main() {
    vector<double> x = { 1, 2, 3, 4 ,5 };
    vector<double> y = { 2, 4, 6, 8, 10 };
    double lr = 0.01;
    double epsilon = 1e-6;
    int max_iter = 10000;
    double w = 1;
    double b = 1;

    int iter = 0;

    // 开始计时
    auto start = high_resolution_clock::now();

    while (iter < max_iter) {
        double dw_val = dw(x, y, w, b);
        double db_val = db(x, y, w, b);

        w -= lr * dw_val;
        b -= lr * db_val;

        double loss_val = loss(x, y, w, b);
        if (abs(dw_val) < epsilon && abs(db_val) < epsilon) break;

        iter++;
    }

    // 结束计时
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Final weight: w = " << w << " b = " << b << endl;
    cout << "Minimum loss = " << loss(x, y, w, b) << endl;
    cout << "Number of iterations: " << iter << endl;
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}
