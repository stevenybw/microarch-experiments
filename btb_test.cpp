#include <iostream>
#include <vector>
#include <random>
#include <x86intrin.h> // 包含__rdtscp

// 定义若干个简单的目标函数
void func0() { asm volatile(""); }
void func1() { asm volatile(""); }
void func2() { asm volatile(""); }
void func3() { asm volatile(""); }
void func4() { asm volatile(""); }
void func5() { asm volatile(""); }
void func6() { asm volatile(""); }
void func7() { asm volatile(""); }

typedef void (*FuncPtr)();

int foo() {
    const int N = 40000000; // 函数调用次数
    unsigned int aux;

    // 不同的函数数量：1, 2, 4, 8
    std::vector<int> func_counts = {1, 2, 4, 8};

    // 定义所有可能的函数指针
    std::vector<FuncPtr> all_funcs = {func0, func1, func2, func3, func4, func5, func6, func7};

    // 随机数生成器
    std::mt19937 rng;
    rng.seed(std::random_device()());

    for (int func_num : func_counts) {
        // 准备函数指针数组
        std::vector<FuncPtr> func_ptrs(N);
        std::uniform_int_distribution<int> dist(0, func_num - 1);

        // 随机填充函数指针数组
        for (int i = 0; i < N; ++i) {
            int idx = dist(rng);
            func_ptrs[i] = all_funcs[idx];
        }

        // 预热缓存，防止冷启动影响
        for (int i = 0; i < N; ++i) {
            func_ptrs[i]();
        }

        // 开始计时
        unsigned long long start = __rdtscp(&aux);
        for (int i = 0; i < N; ++i) {
            func_ptrs[i]();
        }
        unsigned long long end = __rdtscp(&aux);

        unsigned long long total_cycles = end - start;
        double average_cycles = static_cast<double>(total_cycles) / N;

        std::cout << "函数数量: " << func_num
                  << "，总周期数: " << total_cycles
                  << "，平均每次调用周期数: " << average_cycles << std::endl;
    }

    return 0;
}

int main() {
    foo();
    foo();
    foo();
    foo();
    foo();
}
