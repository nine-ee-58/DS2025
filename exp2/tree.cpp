/*
 * @Author       : ee
 * @Date         : 2025-11-20 11:53:17
 * SVC reporting!
 */

#include "../Mystl/btree.h"
#include <bits/stdc++.h>
using namespace std;

class Bitmap {
  private:
    unsigned char *M;
    int N, _sz;

  protected:
    void init(int n) {
      M = new unsigned char[N = (n + 7) / 8];
      memset(M, 0, N);
      _sz = n;
    }

  public:
    Bitmap(int n = 256) { init(n); }
    Bitmap(const char *file, int n = 256) {
      init(n);
      FILE *fp = fopen(file, "rb");
      if (fp) {
        fread(M, sizeof(char), N, fp);
        fclose(fp);
        _sz = 0;
        for (int k = 0; k < n; k++) {
          if (test(k))
            _sz++;
        }
      }
    }
    ~Bitmap() {
      delete[] M;
      M = NULL;
    }
    int size() { return _sz; }
    void set(int k) {
      expand(k);
      if (!test(k))
        _sz++;
      M[k >> 3] |= (0x80 >> (k & 0x07));
    }

    void clear(int k) {
      expand(k);
      if (test(k))
        _sz--;
      M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }

    bool test(int k) {
      expand(k);
      return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    void dump(const char *file) {
      FILE *fp = fopen(file, "wb");
      if (fp) {
        fwrite(M, sizeof(char), N, fp);
        fclose(fp);
      }
    }

    char *bits2string(int n) { // 将前n位转换为字符串
      expand(n - 1);
      char *s = new char[n + 1];
      s[n] = '\0';
      for (int i = 0; i < n; i++) {
        s[i] = test(i) ? '1' : '0';
      }
      return s;
    }

    void expand(int k) { // 扩展位图空间
      if (k < 8 * N)
        return; // 仍在界内，无需扩容
      int oldN = N;
      unsigned char *oldM = M;
      init(2 * (k + 1)); // 扩容为原来的2倍
      if (oldM) {
        memcpy(M, oldM, oldN);
        delete[] oldM;
      }
    }
};

struct HuffNode {     // 哈夫曼树节点结构
    char data;        // 字符数据
    int weight;       // 权重/频率
    HuffNode *left;   // 左子节点
    HuffNode *right;  // 右子节点
    HuffNode *parent; // 父节点
    HuffNode(char c = '\0', int w = 0) : data(c), weight(w), left(nullptr), right(nullptr), parent(nullptr) {}
    bool isLeaf() const { return !left && !right; }
};

// TODO 更改注释
struct NodeCompare { // 比较函数，用于优先队列
    bool operator()(HuffNode *a, HuffNode *b) {
      return a->weight > b->weight; // 权重小的优先级高
    }
};

// 哈夫曼树类
class HuffTree {
  private:
    HuffNode *root;          // 根节点
    string text;             // 原始文本
    map<char, int> freq;     // 字符频率统计
    map<char, string> codes; // 字符编码表
    // 释放子树内存
    void releaseSubtree(HuffNode *node) {
      if (!node)
        return;
      releaseSubtree(node->left);
      releaseSubtree(node->right);
      delete node;
    }

  public:
    HuffTree() : root(nullptr) {}
    ~HuffTree() { releaseSubtree(root); }
    // 统计字符频率
    void calculateFreq(const string &text) {
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
    void buildHuffmanTree(const string &text) {
      this->text = text;
      calculateFreq(text);
      // 创建叶子节点的优先队列
      priority_queue<HuffNode *, vector<HuffNode *>, NodeCompare> pq;
      // 为每个字符创建叶子节点并添加到优先队列
      for (auto &pair : freq) {
        HuffNode *node = new HuffNode(pair.first, pair.second);
        pq.push(node);
      }
      // 构建哈夫曼树
      while (pq.size() > 1) {
        HuffNode *left = pq.top();
        pq.pop();
        HuffNode *right = pq.top();
        pq.pop();
        HuffNode *parent = new HuffNode('\0', left->weight + right->weight);
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
    void generateCodesHelper(HuffNode *node, string code) {
      if (!node)
        return;
      if (node->isLeaf()) {
        codes[node->data] = code.empty() ? "0" : code; // 如果只有一个节点，编码为"0"
      } else {
        generateCodesHelper(node->left, code + "0");
        generateCodesHelper(node->right, code + "1");
      }
    }
    // 获取编码表
    map<char, string> getCodes() { return codes; }
    // 对文本进行编码
    string encode(const string &text) {
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
    string decode(const string &encoded) {
      string decoded = "";
      HuffNode *current = root;
      for (char bit : encoded) {
        if (bit == '0') {
          if (current && current->left) {
            current = current->left;
            if (current->isLeaf()) {
              decoded += current->data;
              current = root; // 回到根节点
            }
          }
        } else if (bit == '1') {
          if (current && current->right) {
            current = current->right;
            if (current->isLeaf()) {
              decoded += current->data;
              current = root; // 回到根节点
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
    int height(HuffNode *node) {
      if (!node)
        return 0;
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
    void printTreeIndentedHelper(HuffNode *node, int depth) {
      if (!node)
        return;
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
    string code;    // 编码字符串
    Bitmap *bitmap; // 使用位图存储编码
  public:
    HuffCode() : bitmap(nullptr) {}
    HuffCode(const string &str) : code(str) {
      bitmap = new Bitmap(code.length());
      for (int i = 0; i < code.length(); i++) {
        if (code[i] == '1') {
          bitmap->set(i);
        }
      }
    }
    ~HuffCode() {
      if (bitmap) {
        delete bitmap;
      }
    }
    void setCode(const string &str) {
      code = str;
      if (bitmap) {
        delete bitmap;
      }
      bitmap = new Bitmap(code.length());
      for (int i = 0; i < code.length(); i++) {
        if (code[i] == '1') {
          bitmap->set(i);
        }
      }
    }
    string getCode() const { return code; }
    // 将编码转换为位图表示
    Bitmap *getBitmap() const { return bitmap; }
    // 从位图恢复编码
    string fromBitmap() const {
      if (!bitmap)
        return "";
      char *bits = bitmap->bits2string(code.length());
      string result(bits);
      delete[] bits;
      return result;
    }
    int length() const { return code.length(); }
    // 重载输出运算符
    friend ostream &operator<<(ostream &os, const HuffCode &hc) {
      os << hc.code;
      return os;
    }
};

int main() {
  // 马丁·路德·金的演讲《I have a dream》的一部分
  string speech = "I have a dream that one day this nation will rise up and live out the "
                  "true meaning of its creed "
                  "We hold these truths to be self-evident that all men are created equal "
                  "I have a dream that one "
                  "day on the red hills of Georgia the sons of former slaves and the sons "
                  "of former slave owners "
                  "will be able to sit down together at the table of brotherhood I have a "
                  "dream that one day even "
                  "the state of Mississippi a state sweltering with the heat of injustice "
                  "sweltering with the heat "
                  "of oppression will be transformed into an oasis of freedom and justice ";
                  
  cout << "原始文本: " << speech << endl << endl;
  // 创建哈夫曼树
  HuffTree huffTree;
  huffTree.buildHuffmanTree(speech);
  cout << "哈夫曼树构建完成！" << endl;
  cout << "树的高度: " << huffTree.height() << endl << endl;
  // 显示哈夫曼编码表
  map<char, string> codes = huffTree.getCodes();
  cout << "字符频率统计:" << endl;
  for (auto &pair : huffTree.getFreq()) {
    cout << "'" << pair.first << "': " << pair.second << " 次" << endl;
  }
  cout << endl;
  cout << "哈夫曼编码表:" << endl;
  for (auto &pair : codes) {
    cout << "'" << pair.first << "': " << pair.second << endl;
  }
  cout << endl;
  // 对特定单词进行编码
  vector<string> testWords = {"I", "have", "dream", "one", "day", "will", "and", "be", "a", "that", "the", "of"};
  cout << "对指定单词进行编码:" << endl;
  for (const string &word : testWords) {
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
    char *bitStr = huffCode.getBitmap()->bits2string(huffCode.length());
    cout << "位图表示: " << bitStr << endl;
    delete[] bitStr;
  }
  cout << endl;
  // 显示哈夫曼树结构（简化版）
  cout << "哈夫曼树结构（部分）:" << endl;
  huffTree.printTree();
  return 0;
}

/*................................................................................................................................................................
................................................................................................................................................................
................................................................................................................................................................
.............................................................................:`，；‘‘~~‘‘；`:......................................................................
....................................................................`~>1lY&RW%NNNN@@NNN%WEKXl+~，`..............................................................
..............................................................`!/I&$%@@NNW$ERKKKKKKKKKRREEWN@@NWK&Yl/!，......................................................
..........................................................‘+IR%@@NWERK&FF&&&KKKKKKKKKKKKKKKKKKKKRRREW%NN%RY/‘..................................................
......................................................；/X$@@NWERREEEEEEEERKKKKKKKKKKKRREEEEEEEEEEERRKK&&KREWNNE*”:..............................................
...................................................!lE@MNEREEERRKKKKKKKREERKKKKREEEERRKKKKKKKKKKRE$ERKKK&&KN@WI”............................................
................................................‘l$@NWEEE$EK&&&&&KKKKKKKKKREEEEEERKKKKKKKKKKKKKKKKKKKR$ERKKKK&&ENMl；.........................................
.............................................:+K@NRE$EK&&&KKKKKKKKKKKKKKKKKKRKKKKKKKKKKKKKKKKKKKKKKKKKREERKKRKK&K$@@F”.......................................
...........................................，]W@WRE$RK&&&KKKKKKKKKKKKKKRRRKKKKKKKKKKRERKKKKKKKKKKKKKKKKKKKKR$RKKRRKK&RNMRi.....................................
.........................................，*N@R$EK&&KRRKKKKKKKKKKKKRE$WWEKKKKKKEWWWERKKKKKKKKKKKKKKKKKKKRERKRRRRKKK%ME>...................................
........................................1NMEWK&&KRRKKKKKKKKKKKKRE$$$ERRRRKKKKKKRRREE$WRKKKKKKKKKKKKKKKKKKKR$RKRRRRRKK%M&!.................................
......................................‘KM%WEKKKRRRRRRRRKKKKKKKREEERKKKKKKKKKKKKKKKKKKKRRE$EKKKKKKKKKKKKKRRKKKKRERRRRRRRKRNMX；...............................
...................................../@M$EKKRRRRRRRRRRRKKKKKKEERKKKKKKKKKKKKKKKKKKKKRRRKKKKKEERKKKKKKKKRRRRRRRRKKREEKRRRRRKRM@}..............................
....................................lM%RRKKRRRRRRRRRRRRRKKKKRERKKKKKRKKKKKKKKKKKKKKKKKKKKREEERRERKKRRRRRRRRRRRRRRRRKR$RRRRRRRRE%ME~............................
...................................XMWKKKRRRRRRRRRRRRRKKEW%NN%%WERKKRKKKRKKKKKKKKKKEW%N@@@@@@NNWEKKRRRRRRRRRRRRRRRKKWRRRRRRKREE@M/...........................
.................................`KMEKRRRRRRRRRRRRRRKRWN@@@@@@@@@@@N%EKRRRKKKKRRKKKN@@@@NNNNNN@@@@NKKRRRRRRRRRRRRRRRKRWERRRRRKRE%MY:.........................
................................i%@KKRRRRRRRRRRRRRKKW@@NNNNNNNNNNNNN@M%KKKRRRRRKKRN@@NNNNNNNNNNNNNNNMNEKRRRRRRRRRRRRRRRRRWEKRRRRKREWMR`........................
..............................:YMWKKRRRRRRRKKKRRRRRN@NNNNNNNNNNNNNNNNNM%WN@@@@NNWN@NNNNNNNNNNNNNNNNNNMMERRRRRRRRRRRRRRRRRRWKRRRRKRW$M$；.......................
.............................~MRKRRKKKKKKKKKKRRRKN@NNNN%%%%%%%%%NNNNN%@M@@@@@MMM@NNNNNNNN%%%%%%NNNNN%M@KRRRRRRRRRRRRRRRRRKWEKRRRRKEW$M%；......................
............................i@@KKRKKKKKKKKKKKKKRKMNNN%%%%%%%%%%%%%%%N%@@%%WWW%N@NNNN%%%%%%%%%%%%%%%%%NMKRRRRRRRRRRRRRRRRRR%EKRRKKR$$EM$`.....................
...........................iM%KKKKKKKKKKKKKKKKKKK%MNN%%%%%WWWWW%%%%%%%%@NERRRRRRNM%%%%%WWWW%%W%%%%%%N%NMEKRRRRRRRRRRRRRRRRRKRWRRRKKKE$$$MK.....................
..........................>M%KRRRRKKKKKKKKKKKKKKKEMN%%%WWWWWWWW$$$WWWNMEKKKKKKKRMNW%%WWWWWWWWWWWWWWWW@MEKRRRRRRRRRRRRRRRRRRKEKRKKKR$$$$M*....................
.........................‘M@KREERKKKKKKKKKKKKKKKK&WM%WWWWWW$$EEEEEEENME&KKKKKKK&EMNWWWWWWWWWWEEEEE%M%WEKRRRRRRRRRRRRRRRRRRKRKRRRKR$$$E%M!...................
.........................KMRREK&KKKKKKKKKKKKK&RKK&WMNW$$$$ERRRRREW@%K&KKKKKKKKK&RN@%$$$$$$EEEEEEW@M%$WEKRRRRKKRKKRRRRRRRRRRRRRRKKW$$$E@W`..................
........................!MWERF&KKKKKKKKKKKKKKKWKKKKFRWN@N%WE$W%NN%E&KKKKKKKKKKKK&KN@@N%WWEEEW%@MN$$$$WRRRKKKKKKKKKKKKKKKKRRRRKRKR%$$$$MY..................
......................:.IM$&XKKKKKKKKKKKKKKK&WR&KK&&&XIFRWWWWWEKXI*YXFFFF&&&&&&&FFFYII&EW%%NNNN%%WW$$$$$$KKKKKKKKKKKEKKKKKKKKKKKRKKRW$$ENM；.................
.......................”@@FFKKKKKKK&K&&KKKK&RW&K&&&&&KXllll***llll********************lllllXKKKKKKK%W$$$$$WRKKKKKKKKKREKKKKKKKKKKKKKKKR%$$$MY.................
......................iMNXFKKKKKKKKKKKKK&KK&WR&&&&&&&&KFI*&X*IIII*****I*************I*FXIIXKKKKKKKR%W$$$$$$$KKKKKKKKKREKKKKKKKKKKKKKKKKR%$$NM；................
.....................}MY&KKKK&&&&&&&KKKKK&RW&&&&&&&&&&&FIRI*I************************RYIFK&&KKKKKR%EEE$$ERKKKKKKKKREKKKKKKKKKKRKKKKKKR%WWM*................
....................*MRI&KKKK&&&&&&&&&&&&&&E$&&&&&&&&&&&&KEI*************************IKX&K&&&&&&K&E@EEEEEEE$KKKKKKKKREKKKKKKKKKKEKKKKKKKWWM$................
...................YM&IFKKK&&&&&&&&&&&&&&&&WR&&&&&&&&&&&FR$&FY********************l**KE&&&&&&&&&K&%MWEEEEEEERKKKKKKKREKKKKKKKKKKKWRKKKKKKKWN@，...............
..................XMFIXKKK&&&&&&&&&&&&&&&&&WK&&&&&&&&&&&FR$&&&FXYI***************IYF&KF&&&&&&&&&RMRNEEEEEEEEE&KKKKK&EEKKKKKKKKKK&E$&KKKKKKKR%M”.:.............
.................&MFIY&KK&$$&&&&&&&&&&&&&FKW&&&&&&&&&&&&FK$&&FF&&FFXXYYIIIIIIYYXF&&FKF&&&&&&&&&FN@”RNREEEEER$$&&KKKK&RKKKKKKKKKKKKWKKKKKKKKKKN&...............
................&MFIXXK&&%MRF&&&&&&&&&&&&FRWF&&&&&&&F&&&&&$&FFFFFFFFFFFFFFFFFFFFFFFXERFFFFFF&&&FRMl‘+MREEEER$$&&&&&&&K&KK&&KKKKKK&RKKKKKKKKKFWE‘.............
...............XMFIXY&&R@MRF&&&&&&&&&&&&&FEF&FFFFFFFFFFFFR&&&FFFXXXXXXXXXXXXXFFFX&W&F&KKKKKKKF@@l}iIMEREEER$$&&&&&&R$&&&&&&&&&&KK&RKKKKKKKKKKFE@>............
..............*M&IXYXFMMF&&&&F&&&&&FFF&FEFFFFFFFFFF&KFKM%FK&&FXXXXXXXXXXXXXXXFFXE$&F&&FFFFFX%@/1lI*MERERR$$&&&&&&ER&&&&&&&&&&KK&RWKKKKKKKKKKRXKM1...........
.............iMEIXXXY%@@M&&&&&&FR&FFFFFF&FRFFFFFFF&KK&&XWN@&XXXXXXXXXXXXXXXXXXXXX&NFFXXXFFFYEM}‘~~~~~IMEKERWEF&&&&&$&&&&&&&&&&&KKFKNKKKKKKKKKKRX&M}..........
............，N%IXXXY%%1@X&&&&F&EFFFFFFFFF&%&FFFF&&&FXXXX@&F@YXXXXXXXXYXYYXXXXXXXYWMWXXXXXXXYRM*‘!!”””!‘lMKRWR&&&&FEEF&&&&&&&&&&&&FK%%WR&KKKKKKKKRY&M1.........
............&MXYXXIW@，XM&F&&&&F&EFFFFFFFFFXEXFFFFXXXXXXXMF!NIXXXXXYYYYYYYYXYYXYKNE%YXXXXXIRMl‘!!”~`，!”‘1NNRW&FFFF&$&F&&&&&&&&&K&FXK%W%KKKKKKKKKKIRM>........
...........~MEIXXIRM”`M%X&FFFFFKFFFFFFFFFXR@EYFXXXFXXXXXMF‘1MRIXYYYXXYYYYYYYYYYYMIXNYXXXYYWM]‘””~，...:‘”!iKNFFFFFEEFFFFFF&&&FR%FFXK%$$WWEKKKKKKKKF*WN，.......
...........}MFYXY&M}.1MRF&FFFFFKW&FFFFFFFFX&@MRYXXXYXXXXY@&!~}MRIYYYXNFIYYYYYYY*$%~&%YXYIF@W+~”~，:.::`:`，~!”@EXFFXRFFFFFFFF&&F%%FFXRN$$EWR&&KK&K&YYMK.......
...........*MXXXY@R..XMFFFFFFFFKWRXFFFFFFFFX%N@EYYKRYYXXYNW!”~/@IYYI$@YYYYYYYI&M/‘R%IIXWMY~‘；`:::``````:..*MXFFXKW&XFFFFFFF&FRN%&FXW%W$$EM@F&&&&YI&M/......
...........1M&YYKM>..KMXFFFFFFFKW%&&K&FXXYYIK%]%%XYNNKFYIRMi~”~>WNXIYXM%YIYYYIIMI!~EEIK%Ki~~‘；:::::..:`~il]WRXFXKW&XXXFFFFFFF&N$%KF&NRW$EM%>R@KFK&YYINW:.....
...........；M%YYWM，..MXFFFF&K&KKKl11111}]]}}l>`XNEX%MREKMK!”~~~YWKY*EM@XIYY*NR!‘~MN@MF”~~‘；；`，；‘”/*F$%@@M%XXXKW&YXXXXXFFFF&%WENKF$%EEEEW$%%”..}NF&XYI&M!.....
............*MKIN@`..$@YXFK&I}//]*YXFXYI***IYYW&:iX%E@K+lIXX!~‘!i}MNRXWFNRI*%W!，.‘Fl*WMMMN%WWW%NMMMM@NW%@RKFRW&YXXXXXXFFX&%WRW%FRNEREEEEM$:....；KNFYYYYM1.....
.............FMFR@`..EMYXF}+]YFFI}+”~‘‘~~~!!~‘”@K:X%%%%ERK&FKEW@MMW/+KMX>RWNE‘:.:....`iIR%@MMMM@%%%NM@WNMR&EW$&YXXXXXXXFXKNWRENERNRREEEREM+...:..F@YYYIM*.....
..............l@WM/..&MFl”IF]i~‘~!i+++i>”””””~>Ml>@NNNM@@@MMMM%Xi:...‘+i..”/:.:::::::...:；”/lX&EMMMR]”*NER%%EFYXXXXXXXYFE%RRW%$%RRREEEERMF.......KWIXY@].....
...............；*$&..lMK”+M+.；‘‘‘；!”!!!!!!”””””‘lM!]YFEEN@MFli；..:`::...:....::::::::::`:::....`/XF；:IMN$MRFYXXXXXXYXKWWRKRR$ERKERREEERM+.::，~iYEXYXM”.....
..................:..‘M%‘/M”`，，`，!>””””!”””””””!‘W$.`，；`*W]，:::`，，`:::::::，`:::::::::::::```，，，，`:::`；11!iWEYYXXXXYYXKW%RKRRRRRRR&XEEREEER@MEKKRE$EFKXXM&......
......................XM>”Mi`；`，~~~~!!””””!~~~!”!/M}:，`，，，:`，，，```::::::::‘`::::...:::::::``````，，，，，，:.>NEYYYYYYX&E%%RWEKRRRRRKFFXRREER%MR&&&FXXYXFKW].......
......................，NR；M].`”]*YYl1i!~‘‘!/*X&RRK%@；l/；，，，，`````::::::::.......:~‘.:::::::``````，，，，`；*N&IYXF&RW$%WKF$%KRRRRKXXFXRRERWMW&KKKREWRX/`........
.......................!%!N&>RN@@NNNN%EF]lKEERRKKRWM]/N!`，````::::::::::.‘”!~!>i+i；.:::::::::``````，，，X@EEE$$$E&&E$&&&K%RKRR&XXFFXRRR%M$&&&&FY*}i‘:...........
........................~‘FM%N%WWWWWWW%NN@%WWWW%%%@N`.:`，```:::::::::::.，~~~‘；`...:::::::::::``````:./%%R&FXYYI&WE&&&&&WRKFXFFFFXERE@@E，......................
............`‘，`:.........+M%W%%%%WWWWWWWWWW%%%%%%W%M!~；....:::`:::::::::.......:::::::::::`:::....`”F%&IYYYYYXEWKF&&&FK@WFFXFFFFFFE%M$&R‘......................
...........`1]]]}1//++>...:N@W%%%%%%WWW%W%WWWWW%%%W%M>FNKI/~，.........:::::::::::::::.........，!+lF%MEYYXXXYXRWE&&&&F&%MMEXFFFFFFXRMY*YW/......................
...........~]>，‘~>}}}li....IM$%%%%%%%%WWWW%%%%%%%%WNM~KWN@NWRF*}+”~；，`.:..........:`，；‘!i}*FEWNN@MWXYXXXYYKWWKF&F&R%@&i@RXFFFFFXXMWl*I*&&......................
...........~li...‘]}1!.....，N@$%%%%%%%%%%%%%%%%%%WNM1，%@RREW%%N@M@NWWW&RK*****FR&&&FFMNN@@M%WN%&YXXXYY&WRFF&KWEI~.1MXXFFFFXX@N**II*Y$‘.....................
...........；]1`..，!；:.......”@@W%%%%%%%%%%%%%%%%WNi..`l$%N%$W%N%%%%@@I”/EE*]IX&l>”!!，‘WNWW%NM@RYXFXYY&$RKKEE$$%‘~1%EXFFFFY&@WI*III*YW>.....................
............i]i..............~$@%%W%%%%%%WW%%%%NNMX......，~+XM@@N%%@%El；~iXM@NME!`~!‘；”&@@MMMMYYFFYXRNN%@%RRK&XI*YREERXFFXYFW@Kl*IIII*YWi.....................
.......:”+/>+}]”...，!>>”‘::，，..1@@@NNN%%%@@@@N@MWMY`lFKX‘1$@NNN%N@MWY*>YWM@MM@MN]；‘>YNMN@MM%FIYXXFE@W>:，/WY**IYYYIIYFXFXXF$@IlIIIIII*XW!.....................
......`}l]]]}}1]‘.~+!，，!+ii>；.+%R&REW%%NN%ERKKKWW$@R；}>~MMMNNNNNMMMMIY]>`‘!>>”!‘l1YMMM@@MMRIYXYF$@WME>..`]EWRXYIYXFFXX&$%MYl*IIIIIII*KE`.....................
......；*]}11}}}l!.``....:``...iNWWWEKERK&KKKKRR%N%!`；~，YRMNNNNM@KFIRXX&FFFFFXI*K@%FKEM@FIYIXE@MMMEYM]...:>IREEEEW%IREl*IIIIIIIIIIW*......................
.......!}}]]]}/~...............`!i/]&ME&KKKKKKKKM”，>>>”F%M@@@NM@@*>*MMMMMMMMMMMMM@M&”>&MXYYIK@MMM@E&R&KM；.:....‘>1*Yl/~.iKIIIIIIIIIII*&W‘......................
........:`；，，`......................`M@K&KKKKKKK&MY:，`::.`%MMMMM%%RWERFE%FWNRE$%NMMKEM&YYX%MMM@KFWMMMY@l..............}EI*IIIII*III*YW1.......................
...................................:.”X$%R&&K&KK&MF.......XFXM@>，`.；:.`.；&/R‘:，.:`；~”+NWYYF@M@@MNMK}WF%K.............lEI*IIII**FFI*IWl........................
......................................i1}E%WREE@W‘......；K!.X&.......:，.}R~`，.......:@RYFM@NNNN@MMl，IM%MR............*EIIIII**XKEI*YW*.........................
....................................‘E~.:!&MK%W>:.......I]..*]:，:......，::`:.....:`:1MIWMNNNN@NMMM%EM@MN‘..........lI*II**F&FYlFW1..........................
...................................‘@].....:@1+M:......，Il:`./K..，，，`...:，:.::....::.RMME%M%%NMK+i]&NMM@@M$`........+WI*I**XRY+FYIEK”...........................
...................................~W*>；:.`.&W`R$；:.../F}.:`:.XK!....~”；.；.`.；}]}}/!+M%%@@M@MM%WWK]!!YM@M@Ml.......；F*I*YEX!~FFKR}:............................
....................................`]FKRF}~%K:，I&K&I1+；:......>&F`~Y]}IYY>；iK1~~>/l&%@@@N@RI&WNNN@%I”NMMNW%；......]WII*KE+:1R&}`..............................
.........................................；I%I`::...‘/FNF`.....:.iW+W>...；+*XE‘.:::...:!1XRl!..，+F%NWN%M@M%NX......IE**EF，.~F}!:................................
........................................”EY`..`；”+}lIYK]:.....:.*NNl.:.`...i/；`:..........；>>，...~YN%NM@M$$$@”...../WIEY........................................
.......................................”M$}IFFFXI]/>~，.........!NW@X..:，``，”‘”+:............:，:....!&MNM%EENX.....:l$&:........................................
.......................................:”//i”；，+l/‘..........:]@%W$/..``..`‘~‘>}.................::.IM@%$$$$N‘......‘`.........................................
...............................................`!}FR&]”；``；>lM%EM/...::...!!~；/!.............:::..IM%%E$$$EN*.................................................
...................................................，”XM@W$%@NW%EE@F.```；`..‘i；‘+!................‘FMWYFWWEEEEW$:................................................
.....................................................FMWEERKKRWEEEM+.:..`~!；”+/i:::::........:~}FEW%FF&YKWEEEN”................................................
....................................................~M%EREE$%NM@%N@@]::；>>‘!F>..........，!/*X&Fl>:&@WFY&YI&$$MI................................................
....................................................iMMNN%KY}!>}}‘‘F&$]；，+WXIi””>/lYF&&F*1!`....”I&EKWWK$KY”................................................
....................................................:i1i!；:..........`!EKFi.`+]***l1i~，.............:；~‘~~；`...................................................
.......................................................................:>i:.....................................................................................
................................................................................................................................................................
................................................................................................................................................................
................................................................................................................................................................
*/