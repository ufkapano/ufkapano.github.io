#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>   // cout
#include <algorithm> // std::find()
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <utility>   // make_pair(), std::pair
#include <cassert>    // assert()

#include "edge.hpp"
#include "basegraph.hpp"
#include "iterator.hpp"

class Graph : public BaseGraph<int> {
    bool directed;
    // Adjacency list to store the graph
    std::vector<std::vector<int> > adj_list;
public:
    // Constructor to initialize the graph with 'n' vertices.
    Graph(int n, bool directed=false) : directed(directed) {
        adj_list = std::vector< std::vector<int> >(n);
        // powstaje wektor z 'n' pustymi wektorami
    }
    ~Graph() { clear(); }
    bool is_directed() const override { return directed; }
    int v() const override { return adj_list.size(); } // liczba wierzcholkow
    int e() const override { // liczba krawedzi
        int counter = 0;
        //int n = adj_list.size();
        //for (int u = 0; u < n; u++)
        //    counter += adj_list[u].size();
        for (auto& vec : adj_list)
            counter += vec.size();
        return (directed ? counter : counter / 2);
    }
    int degree(int u) override { return adj_list[u].size(); }
    int indegree(int u) override { return 0; } // poprawic
    int outdegree(int u) override { return adj_list[u].size(); }
    void add_node(int u) override { assert( 0 <= u && u < v() ); } // tylko test zakresu
    void del_node(int u) override; // usuwanie krawedzi incydentnych
    bool has_node(int u) const override { return (0 <= u && u < v()); } // tylko test zakresu
    void add_edge(int u, int v, float weight=1.0) override; // wstawienie krawędzi (u,v)
    void add_edge(Edge<int> edge) override;
    void del_edge(int u, int v) override; // usunięcie krawędzi (u,v)
    void del_edge(Edge<int> edge) override;
    bool has_edge(int u, int v) const override;
    bool has_edge(Edge<int> edge) const override;
    float weight(int u, int v) const override;
    float weight(Edge<int> edge) const override;
    void clear() override; // usunięcie wszystkich krawędzi
    void display() const override;


    class NodeIterator : public Iterator<int> {
        Graph *gptr;
        int node;
    public:
        NodeIterator(Graph *gp) : gptr(gp), node(0) {}
        NodeIterator(Graph *gp, int u) : gptr(gp), node(u) {}
        ~NodeIterator() {}
        NodeIterator(const NodeIterator& other) {  // copy constructor
            std::cout << "copy constructor NodeIterator\n";
            gptr = other.gptr;
            node = other.node;
        }
        NodeIterator& operator=(const NodeIterator& other) { // copy assignment
            std::cout << "copy assignment NodeIterator\n";
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
        int r_it;
        std::vector<int>::iterator c_it;
    public:
        EdgeIterator(Graph *gp) : gptr(gp) {
            // Domyslny konstruktor ustawi sie na pierwsza istniejaca krawedz.
            // Daje zabezpieczenie, zeby byl jakis wierzcholek.
            // Inaczej jest problem z ustawieniem c_it na end().
            assert( !gptr->adj_list.empty() );
            for (int node = 0; node < gptr->v(); ++node) {
                for (auto a_it = gptr->adj_list[node].begin();
                    a_it != gptr->adj_list[node].end();
                    ++a_it) {
                    if (gptr->is_directed() || (node < *a_it)) {
                        r_it = node;
                        c_it = a_it;
                        return;
                    } // if
                } // for + a_it
            } // for + node
            // Za petla, czyli nie ma krawedzi.
            r_it = gptr->v(); // moja konwencja
            c_it = gptr->adj_list[0].end(); // moja konwencja
        }
        EdgeIterator(Graph *gp,
            int node,
            std::vector<int>::iterator a_it) 
            : gptr(gp), r_it(node), c_it(a_it) {}
        ~EdgeIterator() {}
        EdgeIterator(const EdgeIterator& other) {  // copy constructor
            std::cout << "copy constructor EdgeIterator\n";
            gptr = other.gptr;
            r_it = other.r_it;
            c_it = other.c_it;
        }
        EdgeIterator& operator=(const EdgeIterator& other) { // copy assignment
            std::cout << "copy assignment EdgeIterator\n";
            if (this == &other) return *this;
            gptr = other.gptr;
            r_it = other.r_it;
            c_it = other.c_it;
            return *this;
        }
        EdgeIterator& operator++() { // ++it przedrostkowy
            // Probuje szukac sasiada w tym samym wierszu.
            for (auto a_it = ++c_it;
                a_it != gptr->adj_list[r_it].end();
                ++a_it) {
                if (gptr->is_directed() || (r_it < *a_it)) {
                    // r_it bez zmian
                    c_it = a_it;
                    return *this;
                }
            }
            // Przechodzimy do nowego wiersza.
            for (int node = ++r_it; node < gptr->v(); ++node) {
                for (auto a_it = gptr->adj_list[node].begin();
                    a_it != gptr->adj_list[node].end();
                    ++a_it) {
                    if (gptr->is_directed() || (node < *a_it)) {
                        r_it = node;
                        c_it = a_it;
                        return *this;
                    } // if
                } // for + a_it
            } // for + node
            // Jezeli tu jestesmy, to skonczyly sie krawedzie.
            r_it = gptr->v(); // moja konwencja
            c_it = gptr->adj_list[0].end(); // moja konwencja
            return *this;
        }
        EdgeIterator operator++(int) { // it++ przyrostkowy
            EdgeIterator it = *this;
            // Probuje szukac sasiada w tym samym wierszu.
            for (auto a_it = ++c_it;
                a_it != gptr->adj_list[r_it].end();
                ++a_it) {
                if (gptr->is_directed() || (r_it < *a_it)) {
                    // r_it bez zmian
                    c_it = a_it;
                    return *this;
                }
            }
            // Przechodzimy do nowego wiersza.
            for (int node = ++r_it; node < gptr->v(); ++node) {
                for (auto a_it = gptr->adj_list[node].begin();
                    a_it != gptr->adj_list[node].end();
                    ++a_it) {
                    if (gptr->is_directed() || (node < *a_it)) {
                        r_it = node;
                        c_it = a_it;
                        return *this;
                    } // if
                } // for + a_it
            } // for + node
            // Jezeli tu jestesmy, to skonczyly sie krawedzie.
            r_it = gptr->v(); // moja konwencja
            c_it = gptr->adj_list[0].end(); // moja konwencja
            return it;
        }
        bool operator==(const EdgeIterator& other) const {
            return (gptr == other.gptr) && (r_it == other.r_it) &&
                (c_it == other.c_it);
        }
        bool operator!=(const EdgeIterator& other) const {
            return (gptr != other.gptr) || (r_it != other.r_it) ||
                (c_it != other.c_it);
        }
        //std::pair<int,int> operator*() const { return {r_it, *c_it}; } // *it
        Edge<int> operator*() const override { return Edge<int>(r_it, *c_it); } // *it
    };

    EdgeIterator edge_begin() {
        return EdgeIterator(this);
    }
    EdgeIterator edge_end() {
        return EdgeIterator(this, this->v(), this->adj_list[0].end());
    }


    class AdjacentIterator : public Iterator<int> {
        Graph *gptr;
        int r_it;
        std::vector<int>::iterator c_it;
    public:
        AdjacentIterator(Graph *gp, int node) 
            : gptr(gp), r_it(node), c_it(gptr->adj_list[r_it].begin()) {}
        AdjacentIterator(Graph *gp, int node, std::vector<int>::iterator a_it) 
            : gptr(gp), r_it(node), c_it(a_it) {}
        ~AdjacentIterator() {}
        AdjacentIterator(const AdjacentIterator& other) {  // copy constructor
            std::cout << "copy constructor AdjacentIterator\n";
            gptr = other.gptr;
            r_it = other.r_it;
            c_it = other.c_it;
        }
        AdjacentIterator& operator=(const AdjacentIterator& other) { // copy assignment
            std::cout << "copy assignment AdjacentIterator\n";
            if (this == &other) return *this;
            gptr = other.gptr;
            r_it = other.r_it;
            c_it = other.c_it;
            return *this;
        }
        AdjacentIterator& operator++() { // ++it przedrostkowy
            // Probuje szukac sasiada w tym samym wierszu.
            ++c_it;
            return *this;
        }
        AdjacentIterator operator++(int) { // it++ przyrostkowy
            AdjacentIterator it = *this;
            ++c_it;
            return it;
        }
        bool operator==(const AdjacentIterator& other) const {
            return (gptr == other.gptr) && (r_it == other.r_it) &&
            (c_it == other.c_it);
        }
        bool operator!=(const AdjacentIterator& other) const {
            return (gptr != other.gptr) || (r_it != other.r_it) ||
            (c_it != other.c_it);
        }
        int operator*() const override { return *c_it; } // *it
    };

    AdjacentIterator adj_begin(int u) {
        return AdjacentIterator(this, u, this->adj_list[u].begin());
    }
    AdjacentIterator adj_end(int u) {
        return AdjacentIterator(this, u, this->adj_list[u].end());
    }
};

void Graph::del_node(int u) { // tylko usuwanie krawedzi
    assert((0 <= u) && (u < v()));
    for (auto& vec : adj_list) {
       auto it = std::find(vec.begin(), vec.end(), u);
       if (it != vec.end())
           vec.erase(it);
    }
}

// Function to add an edge between vertices u and v of the graph
void Graph::add_edge(int u, int w, float weight) {
    assert( u >= 0 && u < v() );
    assert( w >= 0 && w < v() );
    for (auto x : adj_list[u])
        assert( w != x ); // no parallel edges
    auto it = std::find(adj_list[u].begin(), adj_list[u].end(), w);
    assert( it == adj_list[u].end() );
    // Add edge from u to v
    adj_list[u].push_back(w);
    // Add edge from v to u if the graph is undirected
    if (!directed)
        adj_list[w].push_back(u);
}

void Graph::del_edge(int u, int w) {
    assert( u >= 0 && u < v() );
    assert( w >= 0 && w < v() );
    auto it = std::find(adj_list[u].begin(), adj_list[u].end(), w);
    assert( it != adj_list[u].end() );
    adj_list[u].erase(it);
    if (!directed) {
        it = std::find(adj_list[w].begin(), adj_list[w].end(), u);
        assert( it != adj_list[w].end() );
        adj_list[w].erase(it);
    }
}

#endif // GRAPH_HPP

// EOF
