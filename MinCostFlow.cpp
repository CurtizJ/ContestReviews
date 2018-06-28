#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 1e9;

struct edge {
    int id, idRev, from, to, capacity, cost, flow;

    edge(int id_, int idRev_, int from_, int to_, int capacity_, int cost_) :
        id(id_), idRev(idRev_), from(from_), to(to_), capacity(capacity_), cost(cost_), flow(0) {}
};

struct Graph {
    int n;
    vector<edge> edges;
    vector<vector<pair<int, int>>> graph;
    
    Graph(int n) : n(n) {
        graph.resize(n);
    }
    
    void addEdge(int from, int to, int capacity, int cost) {
        int id = edges.size();
        edges.push_back(edge(id, id + 1, from, to, capacity, cost));
        edges.push_back(edge(id + 1, id, to, from, 0, -cost));
        graph[from].push_back({to, id});
        graph[to].push_back({from, id + 1});
    }
};

vector<int> getPotentials(const Graph& g, int s) {
    vector<int> dist(g.n, INF);
    dist[s] = 0;
    for(int i = 0; i < g.n - 1; ++i) {
        for(auto e : g.edges) {
            if(e.capacity > e.flow && dist[e.from] < INF && dist[e.from] + e.cost < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.cost;
            }
        }
    }
    return dist;
}

vector<int> getShortestPath(const Graph& g, const vector<int>& p, vector<int>& dist, int s, int t) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    vector<int> prev(g.n, -1);
    vector<bool> used(g.n);
    dist[s] = 0;
    q.push({0, s});
    for(int i = 0; i < g.n; ++i) {
        while(!q.empty() && used[q.top().second]) {
            q.pop();
        }
        if(q.empty()) {
            break;
        }
        int v = q.top().second;
        used[v] = true;
        for(auto to : g.graph[v]) {
            if(g.edges[to.second].capacity == g.edges[to.second].flow) {
                continue;
            }
            int weight = g.edges[to.second].cost + p[v] - p[to.first];
            if(dist[v] + weight < dist[to.first]) {
                dist[to.first] = dist[v] + weight;
                prev[to.first] = to.second;
            }
            q.push({dist[to.first], to.first});
        }
    }
    
    if(dist[t] == INF) {
        return {};
    }
    vector<int> path;
    while(prev[t] != -1) {
        path.push_back(prev[t]);
        t = g.edges[prev[t]].from;
    }
    return path;
}

bool findMinCostFlow(Graph& g, int s, int t, int flowSize) {
    auto p = getPotentials(g, s);
    for(int iter = 0; iter < flowSize; ++iter) {
        vector<int> dist(g.n, INF);
        auto path = getShortestPath(g, p, dist, s, t);
        for(int i = 0; i < g.n; ++i) {
            p[i] += dist[i];
        }
        if(path.empty()) {
            return false;
        }
        int maxFlow = INF;
        for(int i : path) {
            maxFlow = min(maxFlow, g.edges[i].capacity - g.edges[i].flow);
        }
        for(int i : path) {
            g.edges[i].flow += maxFlow;
            g.edges[g.edges[i].idRev].flow -= maxFlow;
        }
    }
    return true;
}

void dfs(int v, int t, Graph& g, vector<int>& path) {
    if(v == t) {
        return;
    }
    for(auto to : g.graph[v]) {
        int id = to.second;
        if(g.edges[id].flow > 0){
            path.push_back(id);
            g.edges[id].flow--;
            dfs(to.first, t, g, path);
            break;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, m, k;
    cin >> n >> m >> k;
    Graph g(n);    
    for(int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        u--; v--;
        g.addEdge(u, v, 1, w);
        g.addEdge(v, u, 1, w);
    }
    if(!findMinCostFlow(g, 0, n-1, k)) {
        cout << "-1\n";
        return 0;
    }
    vector<vector<int>> paths;
    double cost = 0;
    for(int i = 0; i < k; ++i) {
        vector<int> path;
        dfs(0, n-1, g, path);
        for(auto j : path) {
            cost += g.edges[j].cost;
        }
        paths.push_back(path);
    }
    cout.precision(9);
    cout.setf(ios::fixed);
    cout << cost / k << "\n";
    for(auto& p : paths) {
        cout << p.size() << " ";
        for(int i : p) {
            cout << i / 4 + 1 << " ";
        }
        cout << "\n";
    }
    return 0;
}
