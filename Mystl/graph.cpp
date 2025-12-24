#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <climits>
#include <algorithm>
#include <iostream>

namespace mystl {

// 图的边结构
struct Edge {
    int from;       // 起始顶点
    int to;         // 目标顶点
    int weight;     // 权重
    
    Edge(int f = 0, int t = 0, int w = 1) : from(f), to(t), weight(w) {}
    
    bool operator<(const Edge& other) const {
        return weight > other.weight;  // 用于优先队列
    }
};

// 图类定义（使用邻接表）
class Graph {
public:
    std::vector<std::list<Edge>> adj;  // 邻接表
    int vertices;                      // 顶点数
    
    // 构造函数
    Graph(int v) : vertices(v) {
        adj.resize(v);
    }
    
    // 添加无向边
    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back(Edge(u, v, w));
        adj[v].push_back(Edge(v, u, w));
    }
    
    // 添加有向边
    void addDirectedEdge(int u, int v, int w = 1) {
        adj[u].push_back(Edge(u, v, w));
    }
    
    // 打印图
    void printGraph() const {
        for (int i = 0; i < vertices; ++i) {
            std::cout << "顶点 " << i << " 的邻接表: ";
            for (const auto& edge : adj[i]) {
                std::cout << "(" << edge.to << "," << edge.weight << ") ";
            }
            std::cout << std::endl;
        }
    }

    // 深度优先搜索 (DFS)
    void dfs(int start, std::vector<bool>& visited, std::vector<int>& result) {
        visited[start] = true;
        result.push_back(start);
        
        for (const auto& edge : adj[start]) {
            if (!visited[edge.to]) {
                dfs(edge.to, visited, result);
            }
        }
    }
    
    std::vector<int> dfs(int start) {
        std::vector<bool> visited(vertices, false);
        std::vector<int> result;
        dfs(start, visited, result);
        return result;
    }
    
    // 广度优先搜索 (BFS)
    std::vector<int> bfs(int start) {
        std::vector<bool> visited(vertices, false);
        std::vector<int> result;
        std::queue<int> q;
        
        visited[start] = true;
        q.push(start);
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            result.push_back(current);
            
            for (const auto& edge : adj[current]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
        
        return result;
    }
    
    // Prim最小生成树算法
    std::vector<Edge> primMST() {
        std::vector<bool> inMST(vertices, false);
        std::vector<int> minWeight(vertices, INT_MAX);
        std::vector<int> parent(vertices, -1);
        std::vector<Edge> mst;
        
        // 从顶点0开始
        minWeight[0] = 0;
        
        // 优先队列存储 {权重, 顶点}
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, 
                           std::greater<std::pair<int, int>>> pq;
        pq.push({0, 0});
        
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            
            if (inMST[u]) continue;
            inMST[u] = true;
            
            if (parent[u] != -1) {
                mst.push_back(Edge(parent[u], u, minWeight[u]));
            }
            
            for (const auto& edge : adj[u]) {
                int v = edge.to;
                int weight = edge.weight;
                
                if (!inMST[v] && weight < minWeight[v]) {
                    minWeight[v] = weight;
                    parent[v] = u;
                    pq.push({minWeight[v], v});
                }
            }
        }
        
        return mst;
    }
    
    // Dijkstra最短路径算法
    std::vector<int> dijkstra(int start) {
        std::vector<int> dist(vertices, INT_MAX);
        std::vector<bool> visited(vertices, false);
        
        dist[start] = 0;
        
        // 优先队列存储 {距离, 顶点}
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, 
                           std::greater<std::pair<int, int>>> pq;
        pq.push({0, start});
        
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            
            if (visited[u]) continue;
            visited[u] = true;
            
            for (const auto& edge : adj[u]) {
                int v = edge.to;
                int weight = edge.weight;
                
                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return dist;
    }
    
    // 双连通分量查找和关节点查找算法
    void findArticulationPointsAndBCC(std::vector<int>& articulationPoints, 
                                      std::vector<std::vector<Edge>>& bccs) {
        std::vector<int> disc(vertices, -1);
        std::vector<int> low(vertices, -1);
        std::vector<int> parent(vertices, -1);
        std::vector<bool> visited(vertices, false);
        std::stack<Edge> st;
        
        int time = 0;
        
        for (int i = 0; i < vertices; i++) {
            if (disc[i] == -1) {
                findAPUtil(i, disc, low, parent, visited, time, articulationPoints, bccs, st);
            }
        }
        
        // 添加最后一个双连通分量
        if (!st.empty()) {
            std::vector<Edge> bcc;
            while (!st.empty()) {
                bcc.push_back(st.top());
                st.pop();
            }
            bccs.push_back(bcc);
        }
    }
    
private:
    void findAPUtil(int u, std::vector<int>& disc, std::vector<int>& low, 
                    std::vector<int>& parent, std::vector<bool>& visited, 
                    int& time, std::vector<int>& articulationPoints, 
                    std::vector<std::vector<Edge>>& bccs, std::stack<Edge>& st) {
        disc[u] = low[u] = time++;
        visited[u] = true;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            
            if (disc[v] == -1) {
                parent[v] = u;
                st.push(Edge(u, v, edge.weight)); // 将边压入栈
                int children = 0; // 重新计算子节点数量，只针对当前节点的直接子节点
                findAPUtil(v, disc, low, parent, visited, time, articulationPoints, bccs, st);
                
                low[u] = std::min(low[u], low[v]);
                children++;
                
                // 如果根节点有多个子节点，则它是关节点
                if (parent[u] == -1 && children > 1) {
                    articulationPoints.push_back(u);
                }
                
                // 如果根节点不是关节点且low值大于等于发现时间，则它是关节点
                if (parent[u] != -1 && low[v] >= disc[u]) {
                    articulationPoints.push_back(u);
                }
                
                // 如果low值大于发现时间，则找到一个双连通分量
                if (low[v] >= disc[u]) {
                    std::vector<Edge> bcc;
                    Edge e;
                    do {
                        if(st.empty()) break; // 防止栈为空时访问
                        e = st.top();
                        st.pop();
                        bcc.push_back(e);
                    } while (!st.empty() && (e.from != u || e.to != v));
                    bccs.push_back(bcc);
                }
            } 
            // Back edge case
            else if (v != parent[u] && disc[v] < disc[u]) {
                low[u] = std::min(low[u], disc[v]);
                st.push(Edge(u, v, edge.weight));
            }
        }
    }
};

} // namespace mystl

#endif // GRAPH_H