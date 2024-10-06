#include <iostream>
#include <x86intrin.h> // 包含__rdtscp指令
#include <vector>
#include <random>

volatile int sink; // 防止编译器优化

int foo() {
    const int N = 200000000; // 迭代次数

    unsigned int aux;
    unsigned long long start, end;
    int x = 0;

    // **可预测的分支**
    start = __rdtscp(&aux);
    for (int i = 0; i < N; ++i) {
        if (1) { // 条件始终为真
            sink = i;
        } else {
            sink = i + 1;
        }
    }
    end = __rdtscp(&aux);
    unsigned long long cycles_predictable = end - start;
    std::cout << "可预测分支的周期数: " << cycles_predictable << std::endl;

    // **不可预测的分支**
    // 准备随机数据
    std::vector<int> data(N);
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist(0, 1);
    for (int i = 0; i < N; ++i) {
        data[i] = dist(rng);
    }

    start = __rdtscp(&aux);
    for (int i = 0; i < N; ++i) {
        if (data[i]) { // 条件随机为真或假
            sink = i;
        } else {
            sink = i + 1;
        }
    }
    end = __rdtscp(&aux);
    unsigned long long cycles_unpredictable = end - start;
    std::cout << "不可预测分支的周期数: " << cycles_unpredictable << std::endl;

    // **计算分支误预测的平均周期惩罚**
    unsigned long long total_mispredictions = N / 2; // 假设随机数据导致一半的分支被误预测
    unsigned long long penalty = (cycles_unpredictable - cycles_predictable) / total_mispredictions;
    std::cout << "估计的每次分支误预测惩罚: " << penalty << " 个周期" << std::endl;

    return 0;
}

int main() {
    foo();
    foo();
    foo();
    foo();
    foo();
}
