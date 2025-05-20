#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cstdlib>

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

void generate_dot_and_png(const std::string& base_name,
                       int num_nodes,
                       const std::vector<std::string>& node_names,
                       const std::vector<std::vector<int>>& adj) {
  std::string dot_file = "build/" + base_name + ".dot";
  std::string png_file = "build/" + base_name + ".png";
  
  std::ofstream out(dot_file);
  out << "digraph G {\n";
  out << "  rankdir=LR;\n";
  out << "  node [shape=box, style=filled, fillcolor=lightblue];\n\n";
  
  for (int i = 0; i < num_nodes; ++i) {
    out << "  \"" << i << "\" [label=\"" << node_names[i] << "\"];\n";
  }
  out << "\n";
  
  for (int u = 0; u < num_nodes; ++u) {
    for (int v : adj[u]) {
      out << "  \"" << u << "\" -> \"" << v << "\";\n";
    }
  }
  
  out << "}\n";
  out.close();

  std::string command = "dot -Tpng " + dot_file + " -o " + png_file;
  std::system(command.c_str());
}

void ensure_directories_exist() {
  std::filesystem::create_directory("build");
}

void save_to_dat_file(const std::string& filename,
                    int num_nodes,
                    const std::vector<std::string>& node_names,
                    const std::vector<std::vector<int>>& adj) {
  std::ofstream dat_file(filename);
  dat_file << "Number of nodes: " << num_nodes << "\n\n";
  
  dat_file << "Nodes:\n";
  for (int i = 0; i < num_nodes; ++i) {
    dat_file << i << ": " << node_names[i] << "\n";
  }
  dat_file << "\nAdjacency list:\n";
  
  for (int u = 0; u < num_nodes; ++u) {
    if (!adj[u].empty()) {
      dat_file << u << " -> ";
      for (size_t i = 0; i < adj[u].size(); ++i) {
        dat_file << adj[u][i];
        if (i < adj[u].size() - 1) {
          dat_file << ", ";
        }
      }
      dat_file << "\n";
    }
  }
  dat_file.close();
}

int main() {
  ensure_directories_exist();
  
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

  save_to_dat_file("build/pancake_tasks.dat", num_pancake_tasks, pancake_task_names, pancake_adj);
  generate_dot_and_png("pancake_tasks", num_pancake_tasks, pancake_task_names, pancake_adj);

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

  int num_other_nodes = 4;
  std::vector<std::string> other_task_names = {
    "Zadanie A", "Zadanie C", "Zadanie B", "Zadanie D"};
  std::vector<std::vector<int>> other_adj(num_other_nodes);
  other_adj[0].push_back(1); // A -> C
  other_adj[1].push_back(2); // C -> B
  other_adj[2].push_back(3); // B -> D
  other_adj[1].push_back(3); // C -> D

  save_to_dat_file("build/other_tasks.dat", num_other_nodes, other_task_names, other_adj);
  generate_dot_and_png("other_tasks", num_other_nodes, other_task_names, other_adj);

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
