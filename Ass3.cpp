/*
Consider the scenario of a maze. The maze is represented as a grid of cells, where each cell can
be either open or blocked. Each cell in the maze represents a vertex. The goal is to find a path
from the starting point to the goal within a given maze using DFS and BFS
*/

/*
Name: Aranv Khadke
PRN: 124B1B144
Div: C
*/
#include <iostream>
#include <queue>
using namespace std;

class Node {
public:
    int dest;
    int weight;
    Node* next;

    Node(int d, int w, Node* n) {
        dest = d;
        weight = w;
        next = n;
    }
};

class Maze {
    int V;
    Node** adjList;

public:
    Maze(int size) {
        V = size;
        adjList = new Node*[V];

        for (int i = 0; i < V; i++) {
            adjList[i] = NULL;
        }
    }

    void addedge(int u, int v, int wt) {
        Node* newnode = new Node(v, wt, adjList[u]);
        adjList[u] = newnode;

        newnode = new Node(u, wt, adjList[v]);
        adjList[v] = newnode;
    }

    void dfshelper(int u, bool vis[]) {
        cout << u << " ";
        vis[u] = true;

        Node* temp = adjList[u];
        while (temp) {
            if (!vis[temp->dest]) {
                dfshelper(temp->dest, vis);
            }
            temp = temp->next;
        }
    }

    void dfs(int start) {
        bool* vis = new bool[V];

        for (int i = 0; i < V; i++) {
            vis[i] = false;
        }

        dfshelper(start, vis);
        delete[] vis;
    }

    void bfs(int start) {
        queue<int> q;
        bool* vis = new bool[V];

        for (int i = 0; i < V; i++) {
            vis[i] = false;
        }

        q.push(start);
        vis[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            cout << u << " ";

            Node* temp = adjList[u];
            while (temp) {
                if (!vis[temp->dest]) {
                    vis[temp->dest] = true;
                    q.push(temp->dest);
                }
                temp = temp->next;
            }
        }

        delete[] vis;
    }
};

int main() {
    Maze g(5);

    g.addedge(0, 1, 3);
    g.addedge(1, 3, 9);
    g.addedge(1, 2, 5);
    g.addedge(2, 4, 7);

    cout << "DFS: ";
    g.dfs(0);
    cout << endl;
    
    cout << "BFS: ";
    g.bfs(0);
    cout << endl;

    return 0;
}
