# tree.cpp 代码文档

## 概述

`tree.cpp` 是一个 C++ 源文件，实现了二叉树、哈夫曼树、位图(Bitmap)和哈夫曼编码等数据结构和算法。该项目主要实现了数据压缩中使用的哈夫曼编码算法，能够对文本进行编码和解码。

## 类定义

### 1. Bitmap 类（位图类）

Bitmap 类用于表示哈夫曼编码中的位数据。

#### 私有成员

- `unsigned char* M`: 位图存储空间
- `int N`: 字节数
- `int _sz`: 有效位数

#### 公共方法

- `Bitmap(int n = 256)`: 按指定容量创建位图
- `Bitmap(const char* file, int n = 256)`: 从文件读取位图
- `~Bitmap()`: 析构函数，释放内存
- `int size()`: 返回有效位数
- `void set(int k)`: 设置第 k 位为 1
- `void clear(int k)`: 清除第 k 位
- `bool test(int k)`: 测试第 k 位是否为 1
- `void dump(const char* file)`: 将位图导出到文件
- `char* bits2string(int n)`: 将前 n 位转换为字符串
- `void expand(int k)`: 扩展位图空间

### 2. BinNode 类（二叉树节点）

BinNode 类定义了二叉树的基本节点结构。

#### 公共成员

- `char data`: 节点存储的数据
- `int weight`: 节点权重
- `BinNode* left`: 左子节点指针
- `BinNode* right`: 右子节点指针
- `BinNode* parent`: 父节点指针

#### 公共方法

- `BinNode(char c = '\0', int w = 0)`: 构造函数，初始化节点
- `bool isLeaf()`: 判断是否为叶子节点

### 3. BinTree 类（二叉树基类）

BinTree 是二叉树的基类，提供基本的树操作。

#### 保护成员

- `BinNode* root`: 根节点指针

#### 公共方法

- `BinTree()`: 构造函数
- `virtual ~BinTree()`: 虚析构函数
- `void deleteSubtree(BinNode* node)`: 删除子树
- `BinNode* getRoot()`: 获取根节点
- `void setRoot(BinNode* r)`: 设置根节点

### 4. NodeCompare 结构体

NodeCompare 是用于优先队列的比较函数，实现按权重从小到大排序。

### 5. HuffTree 类（哈夫曼树类）

HuffTree 类继承自 BinTree，实现了哈夫曼编码树的构建和操作。

#### 私有成员

- `string text`: 原始文本
- `map<char, int> freq`: 字符频率统计
- `map<char, string> codes`: 字符编码表

#### 公共方法

- `HuffTree()`: 构造函数
- `~HuffTree()`: 析构函数
- `void calculateFreq(const string& text)`: 统计字符频率
- `map<char, int> getFreq() const`: 获取字符频率映射
- `void buildHuffmanTree(const string& text)`: 构建哈夫曼树
- `void generateCodes()`: 生成哈夫曼编码
- `void generateCodesHelper(BinNode* node, string code)`: 生成编码的辅助函数
- `map<char, string> getCodes()`: 获取编码表
- `string encode(const string& text)`: 对文本进行编码
- `string decode(const string& encoded)`: 解码
- `int height(BinNode* node)`: 计算树的高度
- `int height()`: 获取树的高度
- `void printTree()`: 打印树的结构
- `void printTreeHelper(BinNode* node, string prefix, bool isLast)`: 打印树结构的辅助函数

### 6. HuffCode 类（哈夫曼编码类）

HuffCode 类使用位图存储哈夫曼编码。

#### 私有成员

- `string code`: 编码字符串
- `Bitmap* bitmap`: 使用位图存储编码

#### 公共方法

- `HuffCode()`: 默认构造函数
- `HuffCode(const string& str)`: 使用编码字符串构造
- `~HuffCode()`: 析构函数
- `void setCode(const string& str)`: 设置编码
- `string getCode() const`: 获取编码
- `Bitmap* getBitmap() const`: 获取位图表示
- `string fromBitmap() const`: 从位图恢复编码
- `int length() const`: 获取编码长度
- `friend ostream& operator<<(ostream& os, const HuffCode& hc)`: 重载输出运算符

## 哈夫曼树构建过程详解

哈夫曼树的构建过程遵循贪心算法策略，具体步骤如下：

1. **频率统计**：遍历输入文本，统计每个字符出现的频率
2. **创建叶子节点**：为每个不同字符创建一个节点，节点权重为字符频率
3. **初始化优先队列**：将所有叶子节点加入最小堆（优先队列），按权重排序
4. **构建树**：
   - 重复以下步骤直到队列中只剩一个节点：
     - 从队列中取出权重最小的两个节点
     - 创建一个新的内部节点，权重为这两个节点权重之和
     - 将这两个节点作为新节点的左右子节点
     - 将新节点放回优先队列
5. **完成构建**：队列中剩下的唯一节点即为哈夫曼树的根节点

## 主函数功能

主函数演示了哈夫曼编码的完整流程：

1. 使用马丁·路德·金的《I have a dream》演讲文本作为输入
2. 统计字符频率
3. 构建哈夫曼树
4. 生成哈夫曼编码表
5. 对特定单词进行编码
6. 对整个文本进行编码和解码
7. 展示 HuffCode 类的使用
8. 显示哈夫曼树的结构

## 算法复杂度

- 构建哈夫曼树: O(n log n)，其中 n 是不同字符的数量
- 编码: O(m)，其中 m 是文本长度
- 解码: O(k)，其中 k 是编码后字符串的长度

## 关键特性

- 支持大小写字母的处理（转换为小写）
- 使用优先队列构建最优二叉树
- 实现了完整的编码和解码功能
- 包含树结构的可视化输出
- 使用位图优化存储空间
