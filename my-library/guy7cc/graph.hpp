#pragma once
#include <vector>
#include <iostream>

namespace guy7cc {

template <class T = int>
struct Edge {
    int from, to;
    T cost;
    int id;

    Edge() = default;
    Edge(int from, int to, T cost = 1, int id = -1) : from(from), to(to), cost(cost), id(id) {}

    bool operator<(const Edge<T>& other) const {
        return cost < other.cost;
    }
    bool operator>(const Edge<T>& other) const {
        return cost > other.cost;
    }
};

template <class T = int>
struct Graph {
    int n;
    std::vector<std::vector<Edge<T>>> g;
    int edge_count;

    Graph() = default;
    explicit Graph(int n) : n(n), g(n), edge_count(0) {}

    int size() const {
        return n;
    }

    void add_directed_edge(int from, int to, T cost = 1) {
        g[from].emplace_back(from, to, cost, edge_count++);
    }

    void add_undirected_edge(int u, int v, T cost = 1) {
        g[u].emplace_back(u, v, cost, edge_count);
        g[v].emplace_back(v, u, cost, edge_count++);
    }

    void read(int m, int base = 1, bool weight = false, bool directed = false) {
        for (int i = 0; i < m; ++i) {
            int u, v;
            T cost = 1;
            std::cin >> u >> v;
            u -= base;
            v -= base;
            if (weight) std::cin >> cost;
            if (directed) {
                add_directed_edge(u, v, cost);
            } else {
                add_undirected_edge(u, v, cost);
            }
        }
    }

    inline const std::vector<Edge<T>>& operator[](int v) const {
        return g[v];
    }

    inline std::vector<Edge<T>>& operator[](int v) {
        return g[v];
    }
};

} // namespace guy7cc
