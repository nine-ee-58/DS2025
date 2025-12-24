#include "../../Mystl/graph.cpp"  
#include <iostream>

#define ll long long

int main() {
  // 构建图1 (A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7)
  mystl::Graph g1(8);
/*   g1.addEdge(0, 1 ,4); // 
  g1.addEdge(1, 2,12); // 
  g1.addEdge(2, 5,2); // 
  g1.addEdge(5, 7,3); // 
  g1.addEdge(0,3, 6); // */
  g1.addEdge(3, 6, 2);
  g1.addEdge(6, 4, 11);
  g1.addEdge(0, 1, 4);
  g1.addEdge(0, 3, 6);
  g1.addEdge(0, 6, 7);
  g1.addEdge(1, 2, 12);
  g1.addEdge(3, 4, 13);
  g1.addEdge(3, 2, 9);
  g1.addEdge(2, 4, 1);
  g1.addEdge(2, 5, 2);
  g1.addEdge(2, 7, 10);
  g1.addEdge(4, 5, 5);
  g1.addEdge(4, 7, 8);

  g1.addEdge(6, 7, 14);
  g1.addEdge(5, 7, 3);
  std::cout<< "====== 图1的分析 ======" << std::endl;

  // 输出邻接表
  g1.printGraph();
  
  // BFS遍历
  auto bfsResult = g1.bfs(0);
  std::cout << "BFS遍历结果: ";
  for (int vertex : bfsResult) {
    std::cout << char('A' + vertex) << " ";
  }
  std::cout << std::endl;
  
  // DFS遍历
  auto dfsResult = g1.dfs(0);
  std::cout << "DFS遍历结果: ";
  for (int vertex : dfsResult) {
    std::cout << char('A' + vertex) << " ";
  }
  std::cout << std::endl;

  // 最短路径 (Dijkstra)
  auto dist = g1.dijkstra(0);
  std::cout << "从A点出发的最短路径: ";
  for (ll i = 0; i < dist.size(); i++) {
    std::cout << char('A' + i) << ":" << dist[i] << " ";
  }
  std::cout << std::endl;

  // 最小生成树 (Prim)
  auto mst = g1.primMST();
  std::cout << "最小支撑树的边:" << std::endl;
  for (const auto& edge : mst) {
    std::cout << char('A' + edge.from) << " - " << char('A' + edge.to) << std::endl;
  }

  // 构建图2用于双连通分量和关节点测试
  mystl::Graph g2(12);
  g2.addEdge(0, 1);
  g2.addEdge(0, 4);
  g2.addEdge(1, 5);
  g2.addEdge(4, 5);
  g2.addEdge(4, 8);
  g2.addEdge(5, 8);
  g2.addEdge(5, 9);
  g2.addEdge(5, 10);
  g2.addEdge(5, 6);
  g2.addEdge(5, 2);
  g2.addEdge(9, 10);
  g2.addEdge(10, 6);
  g2.addEdge(2, 3);
  g2.addEdge(3, 7);
  g2.addEdge(2, 7);
  g2.addEdge(10, 11);

  std::cout << "\n===== 双连通分量和关节点 ======" << std::endl;
  
  std::vector<int> articulationPoints;
  std::vector<std::vector<mystl::Edge>> bccs;
  g2.findArticulationPointsAndBCC(articulationPoints, bccs);
  
  std::cout << "关节点: ";
  for (int ap : articulationPoints) {
    std::cout << char('A' + ap) << " ";
  }
  std::cout << std::endl;
  
  std::cout << "双连通分量数量: " << bccs.size() << std::endl;
  for (size_t i = 0; i < bccs.size(); i++) {
    std::cout << "双连通分量 " << i + 1 << ": ";
    for (const auto& edge : bccs[i]) {
      std::cout << "(" << char('A' + edge.from) << "-" << char('A' + edge.to) << ") ";
    }
    std::cout << std::endl;
  }

  return 0;
}