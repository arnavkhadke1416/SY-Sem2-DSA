#include <bits/stdc++.h>
using namespace std;

// ======================= EDGE =======================
class Edge {
public:
    int u, v, cost;
    Edge(int u, int v, int cost) : u(u), v(v), cost(cost) {}
};

// ======================= DSU =======================
class DSU {
    vector<int> parent, rnk;

public:
    DSU(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        for(int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if(parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if(a == b) return false;

        if(rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;

        if(rnk[a] == rnk[b]) rnk[a]++;
        return true;
    }
};

// ======================= GRAPH =======================
class Graph {
    int n;
    vector<Edge> edges;
    vector<vector<pair<int,int>>> adj;

    vector<vector<pair<int,int>>> mstTree;
    vector<Edge> mstEdges;
    int mstCost;

    vector<bool> active;

public:
    Graph(int n) {
        this->n = n;
        adj.resize(n);
        active.assign(n, true);
    }

    bool validNode(int x) {
        return x >= 0 && x < n && active[x];
    }

    int activeCount() {
        int cnt = 0;
        for(int i = 0; i < n; i++) {
            if(active[i]) cnt++;
        }
        return cnt;
    }

    // ================= ADD EDGE =================
    void addEdge(int u, int v, int cost) {
        if(!validNode(u) || !validNode(v)) {
            cout << "Invalid or inactive node!\n";
            return;
        }

        if(u == v) {
            cout << "Self-loop not allowed!\n";
            return;
        }

        if(cost < 0) {
            cout << "Cost must be non-negative!\n";
            return;
        }

        for(auto &e : edges) {
            if((e.u == u && e.v == v && e.cost == cost) ||
               (e.u == v && e.v == u && e.cost == cost)) {
                cout << "Duplicate edge!\n";
                return;
            }
        }

        edges.push_back(Edge(u,v,cost));
        adj[u].push_back({v,cost});
        adj[v].push_back({u,cost});

        cout << "Transmission line added successfully.\n";
    }

    // ================= DELETE VERTEX =================
    void deleteVertex(int node) {
        if(node < 0 || node >= n) {
            cout << "Invalid substation!\n";
            return;
        }

        if(!active[node]) {
            cout << "Substation already removed!\n";
            return;
        }

        active[node] = false;

        vector<Edge> newEdges;
        for(auto &e : edges) {
            if(e.u != node && e.v != node) {
                newEdges.push_back(e);
            }
        }
        edges = newEdges;

        adj[node].clear();

        for(int i = 0; i < n; i++) {
            vector<pair<int,int>> temp;
            for(auto &p : adj[i]) {
                if(p.first != node) {
                    temp.push_back(p);
                }
            }
            adj[i] = temp;
        }

        cout << "Substation " << node << " removed.\n";
        cout << "Warning: Grid changed. Recompute MST.\n";
    }

    // ================= SHOW GRAPH =================
    void showGraph() {
        cout << "\nActive Power Grid Network:\n";
        for(int i = 0; i < n; i++) {
            if(!active[i]) continue;

            cout << i << " -> ";
            for(auto &p : adj[i]) {
                if(active[p.first]) {
                    cout << "(" << p.first << "," << p.second << ") ";
                }
            }
            cout << "\n";
        }
    }

    // ================= CONNECTIVITY =================
    bool isConnected() {
        int start = -1;

        for(int i = 0; i < n; i++) {
            if(active[i]) {
                start = i;
                break;
            }
        }

        if(start == -1) return false;

        vector<bool> vis(n, false);
        queue<int> q;

        q.push(start);
        vis[start] = true;

        while(!q.empty()) {
            int u = q.front(); q.pop();

            for(auto &p : adj[u]) {
                int v = p.first;
                if(active[v] && !vis[v]) {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }

        for(int i = 0; i < n; i++) {
            if(active[i] && !vis[i]) return false;
        }

        return true;
    }

    // ================= KRUSKAL =================
    bool buildKruskalMST() {
        if(!isConnected()) {
            cout << "Graph not connected!\n";
            return false;
        }

        vector<Edge> input = edges;
        sort(input.begin(), input.end(),
            [](Edge &a, Edge &b) {
                return a.cost < b.cost;
            });

        DSU dsu(n);
        mstEdges.clear();
        mstTree.assign(n, {});
        mstCost = 0;

        for(auto &e : input) {
            if(!active[e.u] || !active[e.v]) continue;

            if(dsu.unite(e.u, e.v)) {
                mstEdges.push_back(e);
                mstCost += e.cost;

                mstTree[e.u].push_back({e.v, e.cost});
                mstTree[e.v].push_back({e.u, e.cost});
            }
        }

        if(mstEdges.size() != (size_t)(activeCount() - 1)) {
            cout << "MST not possible!\n";
            return false;
        }

        cout << "\nKruskal MST:\n";
        for(auto &e : mstEdges) {
            cout << e.u << " - " << e.v << " cost=" << e.cost << "\n";
        }
        cout << "Total Cost = " << mstCost << "\n";

        return true;
    }

    // ================= PRIM =================
    void buildPrimMST() {
        if(!isConnected()) {
            cout << "Graph not connected!\n";
            return;
        }

        int start = -1;
        for(int i = 0; i < n; i++) {
            if(active[i]) {
                start = i;
                break;
            }
        }

        vector<bool> vis(n, false);

        priority_queue<
            pair<int,pair<int,int>>,
            vector<pair<int,pair<int,int>>>,
            greater<pair<int,pair<int,int>>>
        > pq;

        pq.push({0,{start,-1}});

        int total = 0;

        cout << "\nPrim MST:\n";

        while(!pq.empty()) {
            auto top = pq.top(); pq.pop();

            int cost = top.first;
            int u = top.second.first;
            int parent = top.second.second;

            if(vis[u]) continue;
            vis[u] = true;

            if(parent != -1) {
                cout << parent << " - " << u << " cost=" << cost << "\n";
                total += cost;
            }

            for(auto &p : adj[u]) {
                int v = p.first;
                int w = p.second;

                if(active[v] && !vis[v]) {
                    pq.push({w,{v,u}});
                }
            }
        }

        cout << "Total Cost = " << total << "\n";
    }

    // ================= DIAMETER =================
    int bfsFarthest(int src, int &node) {
        vector<int> dist(n, -1);
        queue<int> q;

        q.push(src);
        dist[src] = 0;

        while(!q.empty()) {
            int u = q.front(); q.pop();

            for(auto &p : mstTree[u]) {
                int v = p.first;
                if(active[v] && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        int mx = 0;
        node = src;

        for(int i = 0; i < n; i++) {
            if(active[i] && dist[i] > mx) {
                mx = dist[i];
                node = i;
            }
        }

        return mx;
    }

    void findDiameter() {
        if(!buildKruskalMST()) return;

        int start = -1;
        for(int i = 0; i < n; i++) {
            if(active[i]) {
                start = i;
                break;
            }
        }

        int node;
        bfsFarthest(start, node);
        int diameter = bfsFarthest(node, node);

        cout << "Maximum Transmission Distance = " << diameter << "\n";
    }
};

// ======================= MAIN =======================
int main() {
    int n;
    cout << "Enter number of substations: ";
    cin >> n;

    Graph g(n);

    while(true) {
        cout << "\n=========================================\n";
        cout << "     POWER GRID TRANSMISSION SYSTEM\n";
        cout << "=========================================\n";
        cout << "1. Add Transmission Line\n";
        cout << "2. Display Grid Network\n";
        cout << "3. Check Grid Connectivity\n";
        cout << "4. Build Minimum Cost Grid (Kruskal)\n";
        cout << "5. Build Minimum Cost Grid (Prim)\n";
        cout << "6. Remove Substation\n";
        cout << "7. Find Maximum Transmission Distance\n";
        cout << "0. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice: ";

        int ch;
        cin >> ch;

        if(ch == 0) {
            cout << "\nSystem shutting down. Goodbye!\n";
            break;
        }

        if(ch == 1) {
            int u, v, c;
            cout << "Enter: Source Destination Cost\n";
            cin >> u >> v >> c;
            g.addEdge(u, v, c);
        }
        else if(ch == 2) {
            g.showGraph();
        }
        else if(ch == 3) {
            cout << (g.isConnected() ? "Grid Connected\n" : "Grid Disconnected\n");
        }
        else if(ch == 4) {
            g.buildKruskalMST();
        }
        else if(ch == 5) {
            g.buildPrimMST();
        }
        else if(ch == 6) {
            int node;
            cout << "Enter substation to remove: ";
            cin >> node;
            g.deleteVertex(node);
        }
        else if(ch == 7) {
            g.findDiameter();
        }
        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}