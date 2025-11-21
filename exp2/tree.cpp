#include <bits/stdc++.h>
#include "../Mystl/btree.h"
using namespace std;

// 位图（Bitmap）类 - 用于表示哈夫曼编码
class Bitmap {
private:
    unsigned char* M;  // 位图存储空间
    int N, _sz;        // N是字节数，_sz是有效位数
protected:
    void init(int n) {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N);
        _sz = n;
    }
public:
    Bitmap(int n = 256) { init(n); }  // 按指定容量创建位图
    Bitmap(const char* file, int n = 256) {  // 从文件读取位图
        init(n);
        FILE* fp = fopen(file, "rb");
        if (fp) {
            fread(M, sizeof(char), N, fp);
            fclose(fp);
            _sz = 0;
            for (int k = 0; k < n; k++) {
                if (test(k)) _sz++;
            }
        }
    }
    ~Bitmap() { 
        delete[] M; 
        M = NULL; 
    }
    int size() { return _sz; }
    void set(int k) {  // 设置第k位为1
        expand(k);
        if (!test(k)) _sz++;
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }
    void clear(int k) {  // 清除第k位
        expand(k);
        if (test(k)) _sz--;
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }
    bool test(int k) {  // 测试第k位是否为1
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }
    void dump(const char* file) {  // 将位图导出到文件
        FILE* fp = fopen(file, "wb");
        if (fp) {
            fwrite(M, sizeof(char), N, fp);
            fclose(fp);
        }
    }
    char* bits2string(int n) {  // 将前n位转换为字符串
        expand(n - 1);
        char* s = new char[n + 1];
        s[n] = '\0';
        for (int i = 0; i < n; i++) {
            s[i] = test(i) ? '1' : '0';
        }
        return s;
    }
    void expand(int k) {  // 扩展位图空间
        if (k < 8 * N) return;  // 仍在界内，无需扩容
        int oldN = N;
        unsigned char* oldM = M;
        init(2 * (k + 1));  // 扩容为原来的2倍
        if (oldM) {
            memcpy(M, oldM, oldN);
            delete[] oldM;
        }
    }
};

// 哈夫曼树节点结构
struct HuffNode {
    char data;        // 字符数据
    int weight;       // 权重/频率
    HuffNode* left;   // 左子节点
    HuffNode* right;  // 右子节点
    HuffNode* parent; // 父节点
    HuffNode(char c = '\0', int w = 0) : data(c), weight(w), left(nullptr), right(nullptr), parent(nullptr) {}
    bool isLeaf() const { return !left && !right; }
};

// 比较函数，用于优先队列
struct NodeCompare {
    bool operator()(HuffNode* a, HuffNode* b) {
        return a->weight > b->weight;  // 权重小的优先级高
    }
};

// 哈夫曼树类
class HuffTree {
private:
    HuffNode* root;   // 根节点
    string text;      // 原始文本
    map<char, int> freq;  // 字符频率统计
    map<char, string> codes;  // 字符编码表
    // 释放子树内存
    void releaseSubtree(HuffNode* node) {
        if (!node) return;
        releaseSubtree(node->left);
        releaseSubtree(node->right);
        delete node;
    }
public:
    HuffTree() : root(nullptr) {}
    ~HuffTree() { releaseSubtree(root); }
    // 统计字符频率
    void calculateFreq(const string& text) {
        freq.clear();
        for (char c : text) {
            // 只处理字母，转换为小写
            if (isalpha(c)) {
                c = tolower(c);
                freq[c]++;
            }
        }
    }
    // 获取字符频率映射
    map<char, int> getFreq() const { return freq; }
    // 构建哈夫曼树
    void buildHuffmanTree(const string& text) {
        this->text = text;
        calculateFreq(text);
        // 创建叶子节点的优先队列
        priority_queue<HuffNode*, vector<HuffNode*>, NodeCompare> pq;
        // 为每个字符创建叶子节点并添加到优先队列
        for (auto& pair : freq) {
            HuffNode* node = new HuffNode(pair.first, pair.second);
            pq.push(node);
        }
        // 构建哈夫曼树
        while (pq.size() > 1) {
            HuffNode* left = pq.top(); pq.pop();
            HuffNode* right = pq.top(); pq.pop();
            HuffNode* parent = new HuffNode('\0', left->weight + right->weight);
            parent->left = left;
            parent->right = right;
            left->parent = parent;
            right->parent = parent;
            pq.push(parent);
        }
        // 设置根节点
        if (!pq.empty()) {
            root = pq.top();
            pq.pop();
        }
        // 生成编码表
        generateCodes();
    }
    // 生成哈夫曼编码
    void generateCodes() {
        codes.clear();
        if (root) {
            string code = "";
            generateCodesHelper(root, code);
        }
    }
    void generateCodesHelper(HuffNode* node, string code) {
        if (!node) return;
        if (node->isLeaf()) {
            codes[node->data] = code.empty() ? "0" : code;  // 如果只有一个节点，编码为"0"
        } else {
            generateCodesHelper(node->left, code + "0");
            generateCodesHelper(node->right, code + "1");
        }
    }
    // 获取编码表
    map<char, string> getCodes() { return codes; }
    // 对文本进行编码
    string encode(const string& text) {
        string encoded = "";
        for (char c : text) {
            if (isalpha(c)) {
                c = tolower(c);
                if (codes.find(c) != codes.end()) {
                    encoded += codes[c] + " ";
                }
            } else {
                // 非字母字符直接保留
                encoded += c;
            }
        }
        return encoded;
    }
    // 解码
    string decode(const string& encoded) {
        string decoded = "";
        HuffNode* current = root;
        for (char bit : encoded) {
            if (bit == '0') {
                if (current && current->left) {
                    current = current->left;
                    if (current->isLeaf()) {
                        decoded += current->data;
                        current = root;  // 回到根节点
                    }
                }
            } else if (bit == '1') {
                if (current && current->right) {
                    current = current->right;
                    if (current->isLeaf()) {
                        decoded += current->data;
                        current = root;  // 回到根节点
                    }
                }
            } else if (bit == ' ') {
                // 空格分隔编码
            } else {
                // 其他字符直接添加
                decoded += bit;
            }
        }
        return decoded;
    }
    // 计算树的高度
    int height(HuffNode* node) {
        if (!node) return 0;
        return 1 + max(height(node->left), height(node->right));
    }
    int height() { return height(root); }
    // 用缩进方式显示树结构（从上到下）
    void printTree() {
        cout << "哈夫曼树结构（从上到下）:" << endl;
        if (root) {
            printTreeIndentedHelper(root, 0);
        }
    }
    void printTreeIndentedHelper(HuffNode* node, int depth) {
        if (!node) return;
        // 打印缩进
        for (int i = 0; i < depth; i++) {
            cout << "  "; // 两个空格作为缩进
        }
        // 打印当前节点
        if (node->isLeaf()) {
            cout << "'" << node->data << "' (freq: " << node->weight << ")" << endl;
        } else {
            cout << "[internal: " << node->weight << "]" << endl;
        }
        // 递归打印子节点
        printTreeIndentedHelper(node->left, depth + 1);
        printTreeIndentedHelper(node->right, depth + 1);
    }
};

// 哈夫曼编码类
class HuffCode {
private:
    string code;  // 编码字符串
    Bitmap* bitmap;  // 使用位图存储编码
public:
    HuffCode() : bitmap(nullptr) {}
    HuffCode(const string& str) : code(str) {
        bitmap = new Bitmap(code.length());
        for (int i = 0; i < code.length(); i++) {
            if (code[i] == '1') {
                bitmap->set(i);
            }
        }
    }
    ~HuffCode() { if (bitmap) { delete bitmap; } }
    void setCode(const string& str) {
        code = str;
        if (bitmap) { delete bitmap; }
        bitmap = new Bitmap(code.length());
        for (int i = 0; i < code.length(); i++) {
            if (code[i] == '1') {
                bitmap->set(i);
            }
        }
    }
    string getCode() const { return code; }
    // 将编码转换为位图表示
    Bitmap* getBitmap() const { return bitmap; }
    // 从位图恢复编码
    string fromBitmap() const {
        if (!bitmap) return "";
        char* bits = bitmap->bits2string(code.length());
        string result(bits);
        delete[] bits;
        return result;
    }
    int length() const { return code.length(); }
    // 重载输出运算符
    friend ostream& operator<<(ostream& os, const HuffCode& hc) {
        os << hc.code;
        return os;
    }
};

// 主函数实现实验要求
int main() {
    // 马丁·路德·金的演讲《I have a dream》的一部分
    string speech = "I have a dream that one day this nation will rise up and live out the true meaning of its creed We hold these truths to be self-evident that all men are created equal I have a dream that one day on the red hills of Georgia the sons of former slaves and the sons of former slave owners will be able to sit down together at the table of brotherhood I have a dream that one day even the state of Mississippi a state sweltering with the heat of injustice sweltering with the heat of oppression will be transformed into an oasis of freedom and justice";
    cout << "=== 哈夫曼编码实验 ===" << endl;
    cout << "原始文本: " << speech << endl << endl;
    // 创建哈夫曼树
    HuffTree huffTree;
    huffTree.buildHuffmanTree(speech);
    cout << "哈夫曼树构建完成！" << endl;
    cout << "树的高度: " << huffTree.height() << endl << endl;
    // 显示哈夫曼编码表
    map<char, string> codes = huffTree.getCodes();
    cout << "字符频率统计:" << endl;
    for (auto& pair : huffTree.getFreq()) {
        cout << "'" << pair.first << "': " << pair.second << " 次" << endl;
    }
    cout << endl;
    cout << "哈夫曼编码表:" << endl;
    for (auto& pair : codes) {
        cout << "'" << pair.first << "': " << pair.second << endl;
    }
    cout << endl;
    // 对特定单词进行编码
    vector<string> testWords = {"dream", "have", "a", "the", "and", "to", "is", "that"};
    cout << "对指定单词进行编码:" << endl;
    for (const string& word : testWords) {
        string encoded = "";
        for (char c : word) {
            c = tolower(c);
            if (codes.find(c) != codes.end()) {
                encoded += codes[c] + " ";
            }
        }
        cout << "单词 '" << word << "': " << encoded << endl;
    }
    cout << endl;
    // 对整个文本进行编码
    string encodedText = huffTree.encode(speech);
    cout << "编码后的文本（部分）:" << endl;
    // 只显示前200个字符
    if (encodedText.length() > 200) {
        cout << encodedText.substr(0, 200) << "..." << endl;
    } else {
        cout << encodedText << endl;
    }
    cout << endl;
    // 演示解码过程
    string decodedText = huffTree.decode(encodedText);
    cout << "解码后的文本（部分）:" << endl;
    if (decodedText.length() > 100) {
        cout << decodedText.substr(0, 100) << "..." << endl;
    } else {
        cout << decodedText << endl;
    }
    cout << endl;
    // 演示HuffCode类的使用
    cout << "演示HuffCode类功能:" << endl;
    HuffCode huffCode(codes['a']);
    cout << "字符'a'的编码: " << huffCode << endl;
    cout << "编码长度: " << huffCode.length() << endl;
    if (huffCode.getBitmap()) {
        char* bitStr = huffCode.getBitmap()->bits2string(huffCode.length());
        cout << "位图表示: " << bitStr << endl;
        delete[] bitStr;
    }
    cout << endl;
    // 显示哈夫曼树结构（简化版）
    cout << "哈夫曼树结构（部分）:" << endl;
    huffTree.printTree();
    return 0;
}