#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#define MAX 9999 // 定义一个很大的数值，表示两点之间没有直接路径
#define MAXVertex 100 // 图中允许的最大顶点数量

// 图的邻接矩阵存储表示
typedef struct
{
    int n; // 图的顶点个数
    string vexs[MAXVertex]; // 顶点信息，直接存储顶点名字
    float arcs[MAXVertex][MAXVertex]; // 邻接矩阵，用于存储边权值
    bool visited[10]={0};// 判断某顶点是否已在最短路径集合中的布尔数组
} GraphMatrix;

// 存储迪杰斯特拉算法中的路径信息
typedef struct
{
    string vertex; // 顶点信息
    float length; // 最短路径长度
    int prevex; // 该顶点的前驱顶点索引
} Path;


// 初始化迪杰斯特拉算法的辅助数据结构
void init(GraphMatrix graph, Path dist[])
{
    int i;
    dist[0].length = 0; // 源点到自己的距离为0
    dist[0].prevex = 0; // 源点没有前驱
    dist[0].vertex = graph.vexs[0]; // 源点的信息
    graph.visited[0] = true; // 源点加入最短路径集合

    // 初始化其他顶点到源点的距离
    for (i = 1; i < graph.n; i++)
    {
        dist[i].length = graph.arcs[0][i]; // 初始距离为邻接矩阵中的值
        dist[i].vertex = graph.vexs[i]; // 顶点信息
        if (dist[i].length != MAX)
            dist[i].prevex = 0; // 如果有路径，则前驱为源点
        else
            dist[i].prevex = -1; // 否则前驱不存在
    }
}

// 初始化图的邻接矩阵表示
void initgraph(GraphMatrix& graph, int length,char args[],char kwargs[])
{
    int i, j;
    map<string, int> point_map; // 点名与索引的映射表
    ifstream points_file("args"); // 读取点文件
    ifstream path_file("kwargs"); // 读取路径文件
    string line, name, from, to;
    float distance;

    // 初始化邻接矩阵中的边信息
    for (i = 0; i < graph.n; i++)
        for (j = 0; j < graph.n; j++)
            graph.arcs[i][j] = (i == j ? 0 : MAX); // 自环边为0，其它默认无穷大

    // 读取点信息文件并构建点名到索引的映射
    if (points_file.is_open())
    {
        getline(points_file, line); // 跳过表头
        while (getline(points_file, line))
        {
            stringstream ss(line);
            getline(ss, name, ','); // 读取点名
            ss >> i; // 读取点ID
            point_map[name] = i - 1; // 点名映射到索引，索引从0开始
            graph.vexs[i - 1] = name; // 直接存储点名
        }
        points_file.close();
    }
    else
    {
        cerr << "Failed to open points.csv" << endl;
        exit(EXIT_FAILURE);
    }

    // 读取路径信息文件并填充邻接矩阵
    if (path_file.is_open())
    {
        getline(path_file, line); // 跳过表头
        while (getline(path_file, line))
        {
            stringstream ss(line);
            getline(ss, from, '-'); // 起点
            getline(ss, to, ','); // 终点
            ss >> distance; // 距离

            if (point_map.find(from) != point_map.end() && point_map.find(to) != point_map.end())
            {
                int u = point_map[from]; // 起点索引
                int v = point_map[to]; // 终点索引
                graph.arcs[u][v] = distance; // 设置边的权值
            }
            else
            {
                cerr << "Error in path data: " << from << " or " << to << " not found in points.csv" << endl;
                exit(EXIT_FAILURE);
            }
        }
        path_file.close();
    }
    else
    {
        cerr << "Failed to open path.csv" << endl;
        exit(EXIT_FAILURE);
    }

    graph.n = point_map.size(); // 设置图的顶点数量
} 