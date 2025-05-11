#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <fstream>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

std::vector<int> topological_sort(
    int num_nodes,
    const std::vector<std::vector<int>>& adj) {
    std::vector<int> in_degree(num_nodes, 0);
    for (int u = 0; u < num_nodes; ++u) {
        for (int v : adj[u]) {
            in_degree[v]++;
        }
    }

    std::queue<int> q;
    for (int i = 0; i < num_nodes; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    std::vector<int> top_order;
    int count_visited_nodes = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        top_order.push_back(u);
        count_visited_nodes++;

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (count_visited_nodes != num_nodes) {
        std::cerr << "Błąd: Graf zawiera cykl! Nie można ustalić "
                     "kolejności."
                  << std::endl;
        return {};
    }

    return top_order;
}

void generate_dot_file(const std::string& filename,
                      int num_nodes,
                      const std::vector<std::string>& node_names,
                      const std::vector<std::vector<int>>& adj) {
    std::ofstream dot_file(filename);
    dot_file << "digraph G {\n";
    dot_file << "    rankdir=LR;\n";
    dot_file << "    node [shape=box, style=filled, fillcolor=lightblue];\n\n";
    
    for (int i = 0; i < num_nodes; ++i) {
        dot_file << "    " << i << " [label=\"" << node_names[i] << "\"];\n";
    }
    dot_file << "\n";
    
    for (int u = 0; u < num_nodes; ++u) {
        for (int v : adj[u]) {
            dot_file << "    " << u << " -> " << v << ";\n";
        }
    }
    
    dot_file << "}\n";
    dot_file.close();
}

void generate_graph_png(const std::string& filename,
                       int num_nodes,
                       const std::vector<std::string>& node_names,
                       const std::vector<std::vector<int>>& adj) {
    Agraph_t* g = agopen(const_cast<char*>("G"), Agdirected, nullptr);
    
    agattr(g, AGRAPH, const_cast<char*>("rankdir"), const_cast<char*>("LR"));
    agattr(g, AGNODE, const_cast<char*>("shape"), const_cast<char*>("box"));
    agattr(g, AGNODE, const_cast<char*>("style"), const_cast<char*>("filled"));
    agattr(g, AGNODE, const_cast<char*>("fillcolor"), const_cast<char*>("lightblue"));
    agattr(g, AGNODE, const_cast<char*>("fontname"), const_cast<char*>("Arial"));
    agattr(g, AGNODE, const_cast<char*>("fontsize"), const_cast<char*>("12"));
    agattr(g, AGEDGE, const_cast<char*>("fontname"), const_cast<char*>("Arial"));
    agattr(g, AGEDGE, const_cast<char*>("fontsize"), const_cast<char*>("10"));
    
    std::vector<Agnode_t*> nodes(num_nodes);
    for (int i = 0; i < num_nodes; ++i) {
        nodes[i] = agnode(g, const_cast<char*>(node_names[i].c_str()), 1);
    }
    
    for (int u = 0; u < num_nodes; ++u) {
        for (int v : adj[u]) {
            agedge(g, nodes[u], nodes[v], nullptr, 1);
        }
    }
    
    GVC_t* gvc = gvContext();
    gvLayout(gvc, g, const_cast<char*>("dot"));
    gvRenderFilename(gvc, g, const_cast<char*>("png"), filename.c_str());
    
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}

int main() {
    int num_pancake_tasks = 6;
    std::vector<std::string> pancake_task_names = {
        "Nagrzać patelnię",
        "Zmieszać jajko, olej, mleko i proszek do naleśników",
        "Wylać część ciasta naleśnikowego na gorącą patelnię",
        "Gdy naleśnik jest rumiany od spodu należy przewrócić i podpiec z "
        "drugiej strony",
        "Podgrzać syrop klonowy",
        "Zjeść rumiany (z obydwu stron) naleśnik polany ciepłym syropem "
        "klonowym"};

    std::vector<std::vector<int>> pancake_adj(num_pancake_tasks);
    pancake_adj[0].push_back(2); // Nagrzać patelnię -> Wylać ciasto
    pancake_adj[1].push_back(2); // Zmieszać składniki -> Wylać ciasto
    pancake_adj[2].push_back(3); // Wylać ciasto -> Przewrócić i podpiec
    pancake_adj[3].push_back(5); // Przewrócić i podpiec -> Zjeść
    pancake_adj[4].push_back(5); // Podgrzać syrop -> Zjeść

    generate_graph_png("build/pancake_tasks.png", num_pancake_tasks, pancake_task_names, pancake_adj);

    std::cout << "--- Proces robienia naleśników ---" << std::endl;
    std::cout << "Reprezentacja procesu jako graf G:" << std::endl;
    std::cout << "Węzły (czynności):" << std::endl;
    for (int i = 0; i < num_pancake_tasks; ++i) {
        std::cout << i << ": " << pancake_task_names[i] << std::endl;
    }
    std::cout << "\nKrawędzie (zależności 'czynność U -> czynność V' "
                 "oznacza, że U musi być wykonane przed V):"
              << std::endl;
    for (int u = 0; u < num_pancake_tasks; ++u) {
        if (!pancake_adj[u].empty()) {
            for (int v : pancake_adj[u]) {
                std::cout << u << " (" << pancake_task_names[u] << ") -> "
                          << v << " (" << pancake_task_names[v] << ")"
                          << std::endl;
            }
        }
    }
    std::cout << std::endl;

    std::cout << "Sugerowana kolejność wykonywania czynności przy robieniu "
                 "naleśników:"
              << std::endl;
    std::vector<int> ordered_pancake_tasks =
        topological_sort(num_pancake_tasks, pancake_adj);

    if (!ordered_pancake_tasks.empty()) {
        for (int task_index : ordered_pancake_tasks) {
            std::cout << "- " << pancake_task_names[task_index] << std::endl;
        }
    } else if (num_pancake_tasks > 0) {
        std::cout
            << "Nie udało się ustalić kolejności (graf może zawierać cykl)."
            << std::endl;
    } else {
        std::cout << "Brak czynności do wykonania." << std::endl;
    }

    std::cout << "\n\n--- Uogólnienie programu ---" << std::endl;
    std::cout
        << "Funkcja 'topological_sort(num_nodes, adj)' jest ogólna i "
           "przyjmuje dowolny graf G \nreprezentowany przez liczbę węzłów "
           "oraz listę sąsiedztwa."
        << std::endl;
    std::cout << "Przykład użycia dla innego, prostego grafu zadań:"
              << std::endl;

    int num_other_nodes = 4;
    std::vector<std::string> other_task_names = {
        "Zadanie A", "Zadanie B", "Zadanie C", "Zadanie D"};
    std::vector<std::vector<int>> other_adj(num_other_nodes);
    other_adj[0].push_back(1); // A -> B
    other_adj[0].push_back(2); // A -> C
    other_adj[1].push_back(3); // B -> D
    other_adj[2].push_back(3); // C -> D

    generate_graph_png("build/other_tasks.png", num_other_nodes, other_task_names, other_adj);

    std::cout << "Węzły przykładowego grafu:" << std::endl;
    for (int i = 0; i < num_other_nodes; ++i) {
        std::cout << i << ": " << other_task_names[i] << std::endl;
    }
    std::cout << "Krawędzie przykładowego grafu:" << std::endl;
     for (int u = 0; u < num_other_nodes; ++u) {
        if (!other_adj[u].empty()) {
            for (int v : other_adj[u]) {
                std::cout << u << " (" << other_task_names[u] << ") -> "
                          << v << " (" << other_task_names[v] << ")"
                          << std::endl;
            }
        }
    }

    std::cout << "\nSugerowana kolejność dla przykładowego grafu:"
              << std::endl;
    std::vector<int> other_ordered_tasks =
        topological_sort(num_other_nodes, other_adj);

    if (!other_ordered_tasks.empty()) {
        for (int task_index : other_ordered_tasks) {
            std::cout << "- " << other_task_names[task_index] << std::endl;
        }
    } else if (num_other_nodes > 0) {
        std::cout << "Nie udało się ustalić kolejności dla przykładowego "
                     "grafu (możliwy cykl)."
                  << std::endl;
    }

    return 0;
}

