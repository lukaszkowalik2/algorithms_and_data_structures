#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

std::vector<std::string> get_movements() {
    return {
        "A->B", "A->C", "A->D",
        "B->A", "B->C", "B->D",
        "D->A", "D->B", "D->C",
        "E->A", "E->B", "E->C", "E->D"
    };
}

bool do_conflict(const std::string &m1, const std::string &m2) {
    char f1 = m1[0], t1 = m1[3];
    char f2 = m2[0], t2 = m2[3];
    bool e1 = (f1 == 'E'), e2 = (f2 == 'E');

    if (e1 != e2) return true;
    if (e1 && e2) return false;
    if (f1 == f2) return true;
    if (t1 == t2) return true;
    if (f1 == t2 && t1 == f2) return true;

    static const std::set<std::pair<std::string, std::string>> cross_conflicts = {
        {"A->C","B->D"}, {"A->C","D->B"}, {"B->D","D->B"},
        {"A->D","B->A"}, {"A->D","E->B"}, {"A->D","E->C"},
        {"B->A","D->C"}, {"B->A","E->C"}, {"D->C","E->B"}
    };
    if (cross_conflicts.count({m1,m2}) || cross_conflicts.count({m2,m1}))
        return true;

    return false;
}

std::map<std::string,std::set<std::string>>
build_conflict_graph(const std::vector<std::string> &moves)
{
    std::map<std::string,std::set<std::string>> graph;
    for (auto &m : moves) {
        graph[m] = {};
    }
    int n = moves.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (do_conflict(moves[i], moves[j])) {
                graph[moves[i]].insert(moves[j]);
                graph[moves[j]].insert(moves[i]);
            }
        }
    }
    return graph;
}

std::map<std::string,int>
color_graph(const std::map<std::string,std::set<std::string>> &graph,
            const std::vector<std::string> &vertices)
{
    std::vector<std::string> order = vertices;
    std::sort(order.begin(), order.end(),
        [&](auto &a, auto &b){
            return graph.at(a).size() > graph.at(b).size();
        });

    std::map<std::string,int> color;
    for (auto &v : order) {
        std::set<int> used;
        for (auto &nei : graph.at(v)) {
            if (color.count(nei)) {
                used.insert(color.at(nei));
            }
        }
        int c = 1;
        while (used.count(c)) ++c;
        color[v] = c;
    }
    return color;
}

int main() {
    std::vector<std::string> movements = get_movements();

    auto conflict_graph = build_conflict_graph(movements);

    auto phase_of = color_graph(conflict_graph, movements);

    int max_phase = 0;
    for (auto &kv : phase_of) {
        if (kv.second > max_phase)
            max_phase = kv.second;
    }

    std::cout << "Minimalna liczba faz: " << max_phase << "\n\n";
    for (int p = 1; p <= max_phase; ++p) {
        std::cout << "Faza " << p << ": ";
        bool first = true;
        for (auto &m : movements) {
            if (phase_of[m] == p) {
                if (!first) std::cout << ", ";
                std::cout << m;
                first = false;
            }
        }
        std::cout << "\n";
    }

    std::ofstream dot("charts/phases.dot");
    dot << "digraph Intersection {\n"
        << "  rankdir=LR;\n"
        << "  node [shape=ellipse];\n"
        << "  A; B; C; D; E;\n";
    for (auto &m : movements) {
        dot << "  " << m[0] << " -> " << m[3]
            << " [label=\"phase " << phase_of[m] << "\"];\n";
    }
    dot << "}\n";
    dot.close();

    system("dot -Tpng charts/phases.dot -o charts/phases.png");

    return 0;
}