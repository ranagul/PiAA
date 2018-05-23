#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#define VERTEX char

using namespace std;

struct Edge {
    VERTEX from;
    VERTEX to;
    int capacity;

    Edge(VERTEX _from, VERTEX _to, int _capacity) : from(_from), to(_to), capacity(_capacity) {}
};

struct Graph {

    vector<Edge> edges;

    /**
     * Добавляет емкости к заданному ребру.
     * Если передана отрицательная емкость, соответственно уменьшает значение текущей емкости ребра.
     * Если такого ребра нет, создает ребро с заданной емкостью.
     */
    void add(VERTEX from, VERTEX to, int additionalCapacity) {
        for (auto &edge : edges) {
            if (edge.from == from && edge.to == to) {
                edge.capacity += additionalCapacity;
                return;
            }
        }
        // Если такого ребра нет - создаем его
        Edge e(from, to, additionalCapacity);
        edges.push_back(e);
    }

    vector<Edge> DFS(VERTEX from, VERTEX to) {
        set<VERTEX> visited;
        return DFS(from, to, visited);
    }

    int degree(VERTEX from) {
        int degree = 0;
        for (auto &edge : edges) {
            if (edge.from == from) {
                degree += edge.capacity;
            }
        }
        return degree;
    }

    void print() {
        sort(edges.begin(), edges.end(), [](const Edge &a, const Edge b) {
            return a.from != b.from ?
                   a.from < b.from :
                   a.to < b.to;
        });

        for (auto &edge: edges) {
            cout << edge.from << " " << edge.to << " " << edge.capacity << endl;
        }
    }

    vector<Edge> findCycle() {
        for (auto &edge: edges) {
            set<VERTEX> visited;
            auto cycle = findCycle(edge.from, edge.from, visited);
            if (!cycle.empty()) return cycle;
        }
        vector<Edge> no_cycle;
        return no_cycle;
    }

    void clearNulls() {
        edges.erase(remove_if(edges.begin(), edges.end(), [](const Edge edge) {
            return edge.capacity == 0;
        }), edges.end());
    }

private:

    vector<Edge> findCycle(VERTEX root, VERTEX from, set<VERTEX> &visited) {
        visited.insert(from);
        for (auto &edge:edges) {
            if (edge.from == from && edge.capacity != 0) { // Берем все ребра из `from`
                if (visited.find(edge.to) != visited.end()) { // Если уже были в edge.to
                    if (edge.to == root) { // И это исходная точка
                        vector<Edge> cycle;
                        cycle.push_back(edge);
                        return cycle;
                    }
                } else { // Если еще не были в этой вершине
                    auto cycle = findCycle(root, edge.to, visited); // Пытаемся найти цикл
                    if (!cycle.empty()) { // Если цикл найдем
                        cycle.push_back(edge); // Добавляем текущее ребро
                        return cycle; // И возвращаем цикл
                    }
                }
            }
        }
        /** Если цикла не было найдено - позвращаем пустой путь */
        vector<Edge> no_cycle;
        return no_cycle;
    }

    vector<Edge> DFS(VERTEX from, VERTEX to, set<VERTEX> &visited) {
        visited.insert(from);
        for (auto &edge:edges) {
            if (edge.from == from && edge.capacity != 0) { // Берем все ребра из `from`
                if (edge.to == to) { // Если дошли до искомой точки
                    visited.insert(edge.to); // Добавляем конечную точку как посещенную, чтобы гарантировать выход из рекурсии.
                    vector<Edge> path;
                    path.push_back(edge);
                    return path;
                }
                if (visited.find(edge.to) == visited.end()) { // Если еще не были в этой вершине
                    visited.insert(edge.to);
                    auto path = DFS(edge.to, to, visited); // Пытается найти через нее путь
                    if (!path.empty()) { // Если путь найдем
                        path.push_back(edge); // Добавляем текущее ребро
                        return path; // И возвращаем готовый путь
                    }
                }
            }
        }

        /** Если пути не было найдено - позвращаем пустой путь */
        vector<Edge> path;
        return path;
    }

};

/**
 * Данный метод удаляет циклы из графа.
 */
void dropCycles(Graph &graph) {
    for (auto cycle = graph.findCycle(); !cycle.empty(); cycle = graph.findCycle()) {
        int capacityMin = 1000000000;
        for (auto &edge : cycle) if (capacityMin > edge.capacity) capacityMin = edge.capacity;
        for (auto &edge : cycle) graph.add(edge.from, edge.to, -capacityMin);
    }
}

/**
 * Метод переносит все ребра из одного графа в другой с нулевой проводиостью.
 * Метод нужен чтобы сохранить все ребра, даже несмотря на то, что они обладают нулевой проводимостью.
 */
void copyNulls(const Graph &donor, Graph &recipient) {
    for (auto &edge: donor.edges) {
        recipient.add(edge.from, edge.to, 0);
    }
}

int main() {
    int N;
    cin >> N;

    VERTEX FROM;
    cin >> FROM;

    VERTEX TO;
    cin >> TO;

    Graph graph;

    for (int i = 0, capacity; i < N; ++i) {
        VERTEX from, to;
        cin >> from >> to >> capacity;
        graph.add(from, to, capacity);
    }

    Graph real;
    copyNulls(graph, real);

    for (auto path = graph.DFS(FROM, TO); !path.empty(); path = graph.DFS(FROM, TO)) {
        // Ищем проводимость полученного пути.
        // Она равна минимуму из всех проводимостей пути.
        int capacityMin = 1000000000;
        for (auto &edge : path) if (capacityMin > edge.capacity) capacityMin = edge.capacity;
        // Переносим линию реальной проводимости из старого графа в новый.
        for (auto &edge : path) {
            graph.add(edge.from, edge.to, -capacityMin);
            real.add(edge.from, edge.to, capacityMin);
        }
    }

    dropCycles(real);

    cout << real.degree(FROM) << endl;

    real.print();

}