#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <string>
class Complex {
private:
    double real;
    double imag; 

public:
    // 构造函数
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
    
    // 获取实部
    double getReal() const { return real; }
    
    // 获取虚部
    double getImag() const { return imag; }
    
    // 计算模
    double getModulus() const { return std::sqrt(real * real + imag * imag); }
    
    // 重载相等运算符
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }
    
    // 重载小于运算符（按模排序，模相同按实部排序）
    bool operator<(const Complex& other) const {
        double mod1 = getModulus();
        double mod2 = other.getModulus();
        if (std::abs(mod1 - mod2) < 1e-10) {
            return real < other.real;
        }
        return mod1 < mod2;
    }
    
    // 重载输出运算符
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real;
        if (c.imag >= 0)
            os << "+" << c.imag << "i)";
        else
            os << c.imag << "i)";
        return os;
    }
};

// 置乱函数
void shuffleVector(std::vector<Complex>& vec) {
    for (int i = vec.size(); i  > 0; i--)
    {
      std::swap(vec[i-1],vec[rand()%i]);
    }
}

// 查找函数（实部和虚部均相同）
int findComplex(const std::vector<Complex>& vec, const Complex& target) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// 唯一化函数
void uniqueVector(std::vector<Complex>& vec) {
    auto it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
}

// 起泡排序
void bubbleSort(std::vector<Complex>& vec) {
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (!(vec[j] < vec[j + 1])) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序的合并函数
void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<Complex> L(n1), R(n2);
    
    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            vec[k] = L[i];
            ++i;
        } else {
            vec[k] = R[j];
            ++j;
        }
        ++k;
    }
    
    while (i < n1) {
        vec[k] = L[i];
        ++i;
        ++k;
    }
    
    while (j < n2) {
        vec[k] = R[j];
        ++j;
        ++k;
    }
}

// 归并排序
void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    mergeSort(vec, left, mid);
    mergeSort(vec, mid + 1, right);
    merge(vec, left, mid, right);
}

void mergeSort(std::vector<Complex>& vec) {
    if (vec.empty()) return;
    mergeSort(vec, 0, vec.size() - 1);
}

// 区间查找函数
std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        double mod = c.getModulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

// 生成随机复数向量
std::vector<Complex> generateRandomComplexVector(int size) {
    std::vector<Complex> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    
    for (int i = 0; i < size; ++i) {
        vec.emplace_back(dis(gen), dis(gen));
    }
    return vec;
}

// 打印向量
void printVector(const std::vector<Complex>& vec, const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << ":" << std::endl;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
        if ((i + 1) % 5 == 0) std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main() {
    // 1. 生成随机复数向量
    std::vector<Complex> vec = generateRandomComplexVector(20);
    printVector(vec);
    std::cout<<"随机复数向量生成完毕"<<"\n";
    
    // (1) 测试各种操作
    std::cout << "=== 测aaaa操作 ===" << std::endl;
    
    // 置乱
    shuffleVector(vec);
    printVector(vec, "置乱后的向量");
    
    // 查找
    Complex target = vec[3]; // 取第4个元素作为查找目标
    int index = findComplex(vec, target);
    std::cout << "查找 " << target << " 的位置: " << index << std::endl << std::endl;
    
    // 插入
    Complex newComplex(7.5, -3.2);
    vec.insert(vec.begin() + 2, newComplex);
    printVector(vec, "在位置2插入(7.5-3.2i)后的向量");
    
    // 删除
    vec.erase(vec.begin() + 5);
    printVector(vec, "删除位置5元素后的向量");
    
    // 唯一化（先添加一些重复元素）
    vec.push_back(vec[0]);
    vec.push_back(vec[1]);
    printVector(vec, "添加重复元素后的向量");
    uniqueVector(vec);
    printVector(vec, "唯一化后的向量");
     std::vector<Complex> largeVec = generateRandomComplexVector(5000);
    // (2) 排序效率比较
    std::cout << "=== 排序效率比较 ===" << std::endl;
    
    // 准备三种情况的向量
    std::vector<Complex> sortedVec = largeVec;
    std::sort(sortedVec.begin(), sortedVec.end());
    
    std::vector<Complex> reverseVec = sortedVec;
    std::reverse(reverseVec.begin(), reverseVec.end());
    
    std::vector<Complex> randomVec = vec;
    shuffleVector(randomVec);
    
    // 测试起泡排序
    auto testBubbleSort = [](std::vector<Complex> vec, const std::string& type) {
        clock_t start = clock();
        bubbleSort(vec);
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC * 1000.0;
        std::cout << "起泡排序(" << type << "): " << std::fixed << std::setprecision(15) 
                  << duration << " ms" << std::endl;
    };
    
    // 测试归并排序
    auto testMergeSort = [](std::vector<Complex> vec, const std::string& type) {
        clock_t start = clock();
        mergeSort(vec);
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC * 1000.0;
        std::cout << "归并排序(" << type << "): " << std::fixed << std::setprecision(15) 
                  << duration << " ms" << std::endl;
    };
    
    testBubbleSort(sortedVec, "顺序");
    testBubbleSort(randomVec, "乱序");
    testBubbleSort(reverseVec, "逆序");
    std::cout << std::endl;
    testMergeSort(sortedVec, "顺序");
    testMergeSort(randomVec, "乱序");
    testMergeSort(reverseVec, "逆序");
    std::cout << std::endl;
    
    // (3) 区间查找
    std::cout << "=== 区间查找 ===" << std::endl;
    
    // 先对向量排序
    mergeSort(vec);
    printVector(vec, "排序后的向量（用于区间查找）");
    
    double m1 = 5.0, m2 = 10.0;
    std::vector<Complex> rangeResult = rangeSearch(vec, m1, m2);
    printVector(rangeResult, "模在[" + std::to_string(m1) + ", " + std::to_string(m2) + ")范围内的复数");
    
    // 显示模值
    std::cout << "对应的模值: ";
    for (const auto& c : rangeResult) {
        std::cout << std::fixed << std::setprecision(2) << c.getModulus() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}