#include<bits/stdc++.h>
using namespace std;
class BTNode {
  public:
  string data;
  BTNode* left;
  BTNode* right;

  BTNode(const string& val) : data(val), left(nullptr), right(nullptr) {}
};
class BT {

  private:
  BTNode* root;

  BTNode* buildTree(const vector<string>& tokens, size_t& index)
  {
    if (index >= tokens.size() || tokens[index] == "#") {
      index++;
      return nullptr;
    }
    BTNode* node = new BTNode(tokens[index++]);
    node->left = buildTree(tokens, index);
    node->right = buildTree(tokens, index);
    return node;
  }
  public:
  BT() : root(nullptr) {}
  void buildFromPreorder(const vector<string>& tokens)
  {
    size_t index = 0;
    root = buildTree(tokens, index);
  }

  void inorderTraversal(BTNode* node)
  {
    if (node == nullptr) return;
    inorderTraversal(node->left);
    cout << node->data << " ";
    inorderTraversal(node->right);
  }

  void preorderTraversal(BTNode* node)
  {
    if (node == nullptr) return;
    cout << node->data << " ";
    preorderTraversal(node->left);
    preorderTraversal(node->right);
  }

  void postorderTraversal(BTNode* node)
  {
    if (node == nullptr) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    cout << node->data << " ";
  }

  void displayInorder()
  {
    inorderTraversal(root);
    cout << endl;
  }

  void displayPreorder()
  {
    preorderTraversal(root);
    cout << endl;
  }

  void displayPostorder()
  {
    postorderTraversal(root);
    cout << endl;
  }

  void displayLevelOrder()
  {
    if (!root) {
      cout << endl;
      return;
    }
    queue<BTNode*> q;
    q.push(root);
    while (!q.empty()) {
      BTNode* cur = q.front(); q.pop();
      cout << cur->data << " ";
      if (cur->left) q.push(cur->left);
      if (cur->right) q.push(cur->right);
    }
    cout << endl;
  }

  int height(BTNode* node)
  {
    if (!node) return 0;
    return 1 + max(height(node->left), height(node->right));
  }

  int height()
  {
    return height(root);
  }

  int countLeaves(BTNode* node)
  {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return countLeaves(node->left) + countLeaves(node->right);
  }

  int countLeaves()
  {
    return countLeaves(root);
  }

  void mirror(BTNode* node)
  {
    if (!node) return;
    swap(node->left, node->right);
    mirror(node->left);
    mirror(node->right);
  }

  void mirror()
  {
    mirror(root);
  }

  void deleteNodes(BTNode* node)
  {
    if (!node) return;
    deleteNodes(node->left);
    deleteNodes(node->right);
    delete node;
  }

  ~BT()
  {
    deleteNodes(root);
  }
};

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << "请输入先序序列，使用 # 表示空节点，空格分隔并回车结束，例如: A B # # C # #\n";
  string line;
  if (!std::getline(cin, line)) return 0;
  istringstream iss(line);
  vector<string> tokens;
  string t;
  while (iss >> t) tokens.push_back(t);

  BT tree;
  tree.buildFromPreorder(tokens);

  cout << "Inorder: "; tree.displayInorder();
  cout << "Preorder: "; tree.displayPreorder();
  cout << "Postorder: "; tree.displayPostorder();
  cout << "Level order: "; tree.displayLevelOrder();
  cout << "Height: " << tree.height() << "\n";
  cout << "Leaf count: " << tree.countLeaves() << "\n";

  cout << "Mirror the tree and show inorder: ";
  tree.mirror();
  tree.displayInorder();

  return 0;
}
