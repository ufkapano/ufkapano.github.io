#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>   // cout
#include <vector>
#include <cassert>    // assert()

#include "edge.hpp"
#include "basegraph.hpp"
#include "iterator.hpp"

class Graph : public BaseGraph<int> {
    bool directed;
    // Adjacency matrix to store graph edges.
    std::vector<std::vector<int> > adj_matrix;
public:
    // Constructor to initialize the graph with 'n' vertices.
    // Tu jest alokowana pamiec i wypelnianie zerami.
    Graph(int n, bool d=false) : directed(d) {
        adj_matrix = std::vector< std::vector<int> >(n, std::vector<int>(n, 0));
    }
    ~Graph() { /* std::cout << "~Graph\n"; */
        for (auto& item : adj_matrix)
            item.clear();
        adj_matrix.clear();
    }
    bool is_directed() const override { return directed; }
    int v() const override { return adj_matrix.size(); }
    int e() const override {
        int counter = 0;
        for (int u = 0; u < v(); u++)
            for (int w = 0; w < v(); w++)
                counter += adj_matrix[u][w];
        return (directed ? counter : counter / 2);
    }
    int degree(int u) override;
    int outdegree(int u) override;
    int indegree(int u) override;
    void add_node(int u) override {
        assert( 0 <= u && u < v() );
    }
    void del_node(int u) override { // tylko usuwamy krawedzie incydentne
        for (int w = 0; w < v(); w++) {
            adj_matrix[u][w] = 0;
            adj_matrix[w][u] = 0;
        }
    }
    bool has_node(int u) const override {
        return (0 <= u && u < v());
    }
    void add_edge(int u, int w, float weight=1.0) override;
    void add_edge(Edge<int> edge) override {
        add_edge(edge.source, edge.target, edge.weight);
    }
    void del_edge(int u, int w) override;
    void del_edge(Edge<int> edge) override {
        del_edge(edge.source, edge.target);
    }
    bool has_edge(int u, int w) const override {
        return adj_matrix[u][w] == 1;
    }
    bool has_edge(Edge<int> edge) const override {
        return adj_matrix[edge.source][edge.target] == 1;
    }
    float weight(int u, int w) const override {
        return adj_matrix[u][w];
    }
    float weight(Edge<int> edge) const override {
        return adj_matrix[edge.source][edge.target];
    }
    void clear() override;
    void display() const override;


    class NodeIterator : public Iterator<int> {
        Graph *gptr;
        int node; // biezacy wierzcholek
    public:
        NodeIterator(Graph *gp) : gptr(gp), node(0) {}
        NodeIterator(Graph *gp, int u) : gptr(gp), node(u) {}
        ~NodeIterator() {}
        NodeIterator(const NodeIterator& other) {  // copy constructor
            gptr = other.gptr;
            node = other.node;
        }
        NodeIterator& operator=(const NodeIterator& other) { // copy assignment
            if (this == &other) return *this;
            gptr = other.gptr;
            node = other.node;
            return *this;
        }
        NodeIterator& operator++() { // ++it przedrostkowy
            ++node;
            return *this;
        }
        NodeIterator operator++(int) { // it++ przyrostkowy
            NodeIterator it = *this;
            ++node;
            return it;
        }
        bool operator==(const NodeIterator& other) const {
            return (gptr == other.gptr) && (node == other.node);
        }
        bool operator!=(const NodeIterator& other) const {
            return (gptr != other.gptr) || (node != other.node);
        }
        int operator*() const override { return node; } // *it
    };

    NodeIterator node_begin() { return NodeIterator(this, 0); }
    NodeIterator node_end() { return NodeIterator(this, v()); }


    class EdgeIterator : public Iterator<Edge<int>> {
        Graph *gptr;
        int r, c;
    public:
        EdgeIterator(Graph *gp) : gptr(gp), r(0), c(0) {
            // Domyslny konstruktor ustawi sie na pierwsza istniejaca krawedz.
            // Nie ma krawedzi (0,0), wiec idziemy dalej.
            while (true) {
                ++c;
                if (c == gptr->v()) { // przechodzimy do nowego wiersza
                    ++r; c = 0;
                }
                if (r == gptr->v()) {
                    c = gptr->v(); // ustawiam edge_end()
                    break;
                }
                if (gptr->has_edge(r, c) && (gptr->is_directed() || (r < c)))
                    break;
            }
        }
        EdgeIterator(Graph *gp, int u, int w) : gptr(gp), r(u), c(w) {}
        EdgeIterator(Graph *gp, Edge<int> edge) :
            gptr(gp), r(edge.source), c(edge.target) {}
        ~EdgeIterator() {}
        EdgeIterator(const EdgeIterator& other) {  // copy constructor
            gptr = other.gptr;
            r = other.r;
            c = other.c;
        }
        EdgeIterator& operator=(const EdgeIterator& other) { // copy assignment
            if (this == &other) return *this;
            gptr = other.gptr;
            r = other.r;
            c = other.c;
            return *this;
        }
        EdgeIterator& operator++() { // ++it przedrostkowy
            while (true) {
                ++c;
                if (c == gptr->v()) {
                    ++r; c = 0;
                }
                if (r == gptr->v()) {
                    c = gptr->v(); // ustawiam edge_end()
                    break;
                }
                if (gptr->has_edge(r, c) && (gptr->is_directed() || (r < c)))
                    break;
            }
            return *this;
        }
        EdgeIterator operator++(int) { // it++ przyrostkowy
            EdgeIterator it = *this;
            while (true) {
                ++c;
                if (c == gptr->v()) {
                    ++r; c = 0;
                }
                if (r == gptr->v()) {
                    c = gptr->v(); // ustawiam edge_end()
                    break;
                }
                if (gptr->has_edge(r, c) && (gptr->is_directed() || (r < c)))
                    break;
            }
            return it;
        }
        bool operator==(const EdgeIterator& other) const {
            return (gptr == other.gptr) && (r == other.r) && (c == other.c);
        }
        bool operator!=(const EdgeIterator& other) const {
            return (gptr != other.gptr) || (r != other.r) || (c != other.c);
        }
        Edge<int> operator*() const override { return Edge<int>(r, c); } // *it
    };

    EdgeIterator edge_begin() { return EdgeIterator(this); }
    EdgeIterator edge_end() { return EdgeIterator(this, v(), v()); }


    class AdjacentIterator : public Iterator<int> {
        Graph *gptr;
        int r, c;
    public:
        AdjacentIterator(Graph *gp, int u) : gptr(gp), r(u), c(-1) {
            while (true) {
                ++c;
                if ((c == gptr->v()) || (gptr->has_edge(r, c))) {
                    break;
                }
            }
        }
        AdjacentIterator(Graph *gp, int u, int w) : gptr(gp), r(u), c(w) {}
        ~AdjacentIterator() {}
        AdjacentIterator(const AdjacentIterator& other) {  // copy constructor
            gptr = other.gptr;
            r = other.r;
            c = other.c;
        }
        AdjacentIterator& operator=(const AdjacentIterator& other) { // copy assignment
            if (this == &other) return *this;
            gptr = other.gptr;
            r = other.r;
            c = other.c;
            return *this;
        }
        AdjacentIterator& operator++() { // ++it przedrostkowy
            while (true) {
                ++c;
                if ((c == gptr->v()) || (gptr->has_edge(r, c))) {
                    break;
                }
            }
            return *this;
        }
        AdjacentIterator operator++(int) { // it++ przyrostkowy
            AdjacentIterator it = *this;
            while (true) {
                ++c;
                if ((c == gptr->v()) || (gptr->has_edge(r, c))) {
                    break;
                }
            }
            return it;
        }
        bool operator==(const AdjacentIterator& other) const {
            return (gptr == other.gptr) && (r == other.r) && (c == other.c);
        }
        bool operator!=(const AdjacentIterator& other) const {
            return (gptr != other.gptr) || (r != other.r) || (c != other.c);
        }
        int operator*() const override { return c; } // *it
    };

    AdjacentIterator adj_begin(int u) { return AdjacentIterator(this, u); }
    AdjacentIterator adj_end(int u) { return AdjacentIterator(this, u, v()); }
};

#endif // GRAPH_HPP

// EOF
