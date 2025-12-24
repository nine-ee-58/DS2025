#include <bits/stdc++.h>
#include "../../Mystl/sorting.cpp"
using namespace std;
using namespace std::chrono;

// 边界框结构
struct BBox {
    double x, y, w, h; // 位置和大小
    double confidence; // 置信度
    double area; // 面积
    
    // 默认构造函数
    BBox() : x(0), y(0), w(0), h(0), confidence(0), area(0) {}
    
    BBox(double x, double y, double w, double h, double conf) 
        : x(x), y(y), w(w), h(h), confidence(conf) {
        area = w * h;
    }
    
    // 计算与其他边界框的IoU（交并比）
    double iou(const BBox& other) const {
        double inter_x1 = max(x, other.x);
        double inter_y1 = max(y, other.y);
        double inter_x2 = min(x + w, other.x + other.w);
        double inter_y2 = min(y + h, other.y + other.h);
        
        if (inter_x2 <= inter_x1 || inter_y2 <= inter_y1) {
            return 0.0;
        }
        
        double inter_area = (inter_x2 - inter_x1) * (inter_y2 - inter_y1);
        double union_area = area + other.area - inter_area;
        
        return inter_area / union_area;
    }
};



// 非极大值抑制（NMS）算法
vector<BBox> NMS(vector<BBox> bboxes, double threshold) {
    if (bboxes.empty()) return {};
    
    // 首先按置信度排序（降序）
    Sorting<BBox>::quickSort(bboxes, [](const BBox& a, const BBox& b) {
        return a.confidence > b.confidence; // 按置信度降序排列
    });
    
    vector<BBox> result;
    for (const auto& bbox : bboxes) {
        bool suppress = false;
        for (const auto& kept : result) {
            if (bbox.iou(kept) > threshold) {
                suppress = true;
                break;
            }
        }
        if (!suppress) {
            result.push_back(bbox);
        }
    }
    
    return result;
}

// 数据生成器
class DataGenerator {
public:
    // 随机分布数据生成
    static vector<BBox> generateRandomDistribution(int count) {
        vector<BBox> bboxes;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> pos_dis(0.0, 100.0); // 位置
        uniform_real_distribution<> size_dis(5.0, 20.0); // 大小
        uniform_real_distribution<> conf_dis(0.1, 1.0); // 置信度
        
        for (int i = 0; i < count; i++) {
            double x = pos_dis(gen);
            double y = pos_dis(gen);
            double w = size_dis(gen);
            double h = size_dis(gen);
            double conf = conf_dis(gen);
            
            bboxes.emplace_back(x, y, w, h, conf);
        }
        
        return bboxes;
    }
    
    // 聚集分布数据生成
    static vector<BBox> generateClusterDistribution(int count) {
        vector<BBox> bboxes;
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<> pos_dis(50.0, 15.0); // 聚集在中心位置
        uniform_real_distribution<> size_dis(5.0, 20.0);
        uniform_real_distribution<> conf_dis(0.1, 1.0);
        
        for (int i = 0; i < count; i++) {
            double x = max(0.0, pos_dis(gen));
            double y = max(0.0, pos_dis(gen));
            double w = size_dis(gen);
            double h = size_dis(gen);
            double conf = conf_dis(gen);
            
            bboxes.emplace_back(x, y, w, h, conf);
        }
        
        return bboxes;
    }
};

int main() {
    // 测试不同数量的边界框
    vector<int> testSizes = {100, 500, 1000, 5000, 10000};
    
    cout << "排序算法在NMS中的性能测试" << endl << endl;
    
    for (int size : testSizes) {
        cout << "数据规模: " << size << endl;
        
        // 测试随机分布
        vector<BBox> randomData = DataGenerator::generateRandomDistribution(size);
        vector<BBox> clusterData = DataGenerator::generateClusterDistribution(size);
        
        // 创建排序算法的副本进行测试
        vector<BBox> dataCopy1 = randomData;
        vector<BBox> dataCopy2 = randomData;
        vector<BBox> dataCopy3 = randomData;
        vector<BBox> dataCopy4 = randomData;
        
        // 快排性能测试
        auto start = high_resolution_clock::now();
        Sorting<BBox>::quickSort(dataCopy1, [](const BBox& a, const BBox& b) {
            return a.confidence > b.confidence; // 按置信度降序排列
        });
        auto end = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end - start);
        
        // 归并性能测试
        start = high_resolution_clock::now();
        Sorting<BBox>::mergeSort(dataCopy2, [](const BBox& a, const BBox& b) {
            return a.confidence > b.confidence; // 按置信度降序排列
        });
        end = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end - start);
        
        // 堆排性能测试
        start = high_resolution_clock::now();
        Sorting<BBox>::heapSort(dataCopy3, [](const BBox& a, const BBox& b) {
            return a.confidence > b.confidence; // 按置信度降序排列
        });
        end = high_resolution_clock::now();
        auto duration3 = duration_cast<microseconds>(end - start);
        
        // 插入排序性能测试（仅对小数据集）
        start = high_resolution_clock::now();
        if (size <= 1000) { // 插入排序仅在小数据集上测试
            Sorting<BBox>::insertionSort(dataCopy4, [](const BBox& a, const BBox& b) {
                return a.confidence > b.confidence; // 按置信度降序排列
            });
        }
        end = high_resolution_clock::now();
        auto duration4 = duration_cast<microseconds>(end - start);
        
        cout << "  随机分布 - 快排: " << duration1.count() << " ms" << endl;
        cout << "  随机分布 - 归并: " << duration2.count() << " ms" << endl;
        cout << "  随机分布 - 堆排: " << duration3.count() << " ms" << endl;
        if (size <= 1000) {
            cout << "  随机分布 - 插入: " << duration4.count() << " ms" << endl;
        }
        
        // 聚集分布测试（仅快排，因为NMS本身使用快排）
        vector<BBox> clusterCopy = clusterData;
        start = high_resolution_clock::now();
        Sorting<BBox>::quickSort(clusterCopy, [](const BBox& a, const BBox& b) {
            return a.confidence > b.confidence; // 按置信度降序排列
        });
        end = high_resolution_clock::now();
        auto duration5 = duration_cast<microseconds>(end - start);
        
        cout << "  聚集分布 - 快排: " << duration5.count() << " ms" << endl;
        
        // NMS算法测试（使用随机数据）
        vector<BBox> nmsInput = randomData;
        start = high_resolution_clock::now();
        vector<BBox> nmsResult = NMS(nmsInput, 0.5);
        end = high_resolution_clock::now();
        auto duration6 = duration_cast<microseconds>(end - start);
        
        cout << "  NMS算法 (IoU阈值=0.5): " << duration6.count() << " ms, 输出" 
             << nmsResult.size() << "个边界框" << endl << endl;
    }
    
    return 0;
}