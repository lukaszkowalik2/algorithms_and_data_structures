#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

const int INF = std::numeric_limits<int>::max() / 2;

struct Coordinates {
  double lat;
  double lon;
};

int get_city_id(const std::string &city_name,
                std::map<std::string, int> &city_to_id,
                std::vector<std::string> &id_to_city, int &next_city_id) {
  if (city_to_id.find(city_name) == city_to_id.end()) {
    city_to_id[city_name] = next_city_id;
    id_to_city.push_back(city_name);
    return next_city_id++;
  }
  return city_to_id[city_name];
}

std::vector<int>
reconstruct_path(int u_id, int v_id,
                 const std::vector<std::vector<int>> &next_hop_matrix,
                 const std::vector<std::vector<int>> &dist_matrix) {
  std::vector<int> path_nodes;
  if (dist_matrix[u_id][v_id] == INF) {
    return path_nodes;
  }
  int curr = u_id;
  while (curr != v_id) {
    path_nodes.push_back(curr);
    curr = next_hop_matrix[curr][v_id];
    if (curr == -1) {
      return {};
    }
    if (path_nodes.size() >
        next_hop_matrix.size()) {
      return {};
    }
  }
  path_nodes.push_back(v_id);
  return path_nodes;
}

void generate_dot_file(
    const std::string &filename, const std::vector<int> &path_node_ids,
    const std::vector<std::string> &id_to_city_map,
    const std::map<int, Coordinates> &city_id_to_coords,
    const std::vector<std::tuple<int, int, int>> &all_direct_edges_with_times,
    const std::vector<std::vector<int>> &original_direct_times_matrix) {
  std::ofstream dot_file(filename);
  if (!dot_file.is_open()) {
    std::cerr << "Błąd: Nie można otworzyć pliku DOT " << filename
              << std::endl;
    return;
  }

  dot_file << "graph ShortestPath {\n";
  dot_file << "  layout=fdp;\n";
  dot_file << "  overlap=false;\n";
  dot_file << "  node [shape=ellipse, style=filled, fillcolor=lightgray];\n";

  for (size_t i = 0; i < id_to_city_map.size(); ++i) {
    const std::string &city_name = id_to_city_map[i];
    auto it = city_id_to_coords.find(i);
    dot_file << "  \"" << city_name << "\" [";
    if (it != city_id_to_coords.end()) {
      dot_file << "pos=\"" << it->second.lon << "," << it->second.lat << "!\"";
    } else {
      std::cerr << "Ostrzeżenie: Brak współrzędnych dla miasta: " << city_name
                << ". Zostanie umieszczone automatycznie." << std::endl;
    }

    if (!path_node_ids.empty()) {
      if (static_cast<int>(i) == path_node_ids.front()) {
        dot_file << ", fillcolor=lightblue, color=blue, peripheries=2";
      } else if (static_cast<int>(i) == path_node_ids.back()) {
        dot_file << ", fillcolor=lightgreen, color=green, peripheries=2";
      }
    }
    dot_file << "];\n";
  }

  std::set<std::pair<int, int>> drawn_direct_edges;
  for (const auto &edge_tuple : all_direct_edges_with_times) {
    int u = std::get<0>(edge_tuple);
    int v = std::get<1>(edge_tuple);
    int time = std::get<2>(edge_tuple);

    int n1 = std::min(u, v);
    int n2 = std::max(u, v);

    if (drawn_direct_edges.find({n1, n2}) == drawn_direct_edges.end()) {
      dot_file << "  \"" << id_to_city_map[u] << "\" -- \""
               << id_to_city_map[v] << "\" [color=gray, style=dashed, label=\""
               << time << "\", fontsize=10];\n";
      drawn_direct_edges.insert({n1, n2});
    }
  }

  if (path_node_ids.size() > 1) {
    for (size_t i = 0; i < path_node_ids.size() - 1; ++i) {
      int u_id = path_node_ids[i];
      int v_id = path_node_ids[i + 1];
      std::string city1_name = id_to_city_map[u_id];
      std::string city2_name = id_to_city_map[v_id];
      int segment_time = original_direct_times_matrix[u_id][v_id];

      dot_file << "  \"" << city1_name << "\" -- \"" << city2_name
               << "\" [color=red, penwidth=2.5, label=\"" << segment_time
               << "\", fontcolor=red, fontsize=12];\n";
    }
  }

  dot_file << "}\n";
  dot_file.close();
}

std::string sanitize_filename(const std::string& name) {
    std::string sanitized_name = name;
    std::replace_if(sanitized_name.begin(), sanitized_name.end(),
                    [](char c){ return !std::isalnum(c); }, '_');
    return sanitized_name;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  std::map<std::string, int> city_to_id;
  std::vector<std::string> id_to_city;
  int next_city_id = 0;

  std::vector<std::tuple<std::string, std::string, int>>
      direct_travel_inputs;
  std::map<std::string, Coordinates> city_name_to_coords_input;

  std::cout << "Podaj dane przejazdów w formacie: Miasto1 Miasto2 Czas (w "
               "minutach).\n";
  std::cout
      << "Zakończ wprowadzanie danych pustą linią lub EOF (Ctrl+D/Ctrl+Z).\n";
  std::string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    std::stringstream ss(line);
    std::string city1_name, city2_name;
    int time;
    if (ss >> city1_name >> city2_name >> time) {
      direct_travel_inputs.emplace_back(city1_name, city2_name, time);
    } else if (!line.empty()) {
      std::cerr << "Błędny format linii (czasy): " << line << std::endl;
    }
  }
  std::cin.clear(); 

  std::cout << "\nPodaj dane współrzędnych geograficznych miast.\n";
  std::cout << "Format: NazwaMiasta SzerokoscGeo DlugoscGeo (np. Warszawa "
               "52.2297 21.0122)\n";
  std::cout
      << "Zakończ wprowadzanie danych pustą linią lub EOF (Ctrl+D/Ctrl+Z).\n";
  while (std::getline(std::cin, line) && !line.empty()) {
    std::stringstream ss(line);
    std::string city_name;
    double lat, lon;
    if (ss >> city_name >> lat >> lon) {
      city_name_to_coords_input[city_name] = {lat, lon};
    } else if (!line.empty()) {
      std::cerr << "Błędny format linii (współrzędne): " << line << std::endl;
    }
  }

  for (const auto &travel : direct_travel_inputs) {
    get_city_id(std::get<0>(travel), city_to_id, id_to_city, next_city_id);
    get_city_id(std::get<1>(travel), city_to_id, id_to_city, next_city_id);
  }
  for (const auto &coord_pair : city_name_to_coords_input) {
    get_city_id(coord_pair.first, city_to_id, id_to_city, next_city_id);
  }

  int num_cities = id_to_city.size();
  if (num_cities == 0) {
    std::cout << "Nie podano żadnych danych o miastach." << std::endl;
    return 0;
  }

  std::map<int, Coordinates> city_id_to_coords;
  for (const auto &pair : city_name_to_coords_input) {
    if (city_to_id.count(pair.first)) {
      city_id_to_coords[city_to_id[pair.first]] = pair.second;
    }
  }

  std::vector<std::vector<int>> adj_matrix(
      num_cities, std::vector<int>(num_cities, INF));
  std::vector<std::vector<int>> original_direct_times_matrix(
      num_cities, std::vector<int>(num_cities, INF));
  std::vector<std::vector<int>> next_hop_matrix(
      num_cities, std::vector<int>(num_cities, -1));
  std::vector<std::tuple<int, int, int>> all_direct_edges_for_dot;


  for (int i = 0; i < num_cities; ++i) {
    adj_matrix[i][i] = 0;
    original_direct_times_matrix[i][i] = 0;
    next_hop_matrix[i][i] = i;
  }

  for (const auto &travel : direct_travel_inputs) {
    int u = city_to_id[std::get<0>(travel)];
    int v = city_to_id[std::get<1>(travel)];
    int time = std::get<2>(travel);

    if (time < adj_matrix[u][v]) {
        adj_matrix[u][v] = time;
        original_direct_times_matrix[u][v] = time;
        next_hop_matrix[u][v] = v;
    }
    if (time < adj_matrix[v][u]) {
        adj_matrix[v][u] = time;
        original_direct_times_matrix[v][u] = time;
        next_hop_matrix[v][u] = u;
    }
    all_direct_edges_for_dot.emplace_back(u, v, time);
  }

  for (int k = 0; k < num_cities; ++k) {
    for (int i = 0; i < num_cities; ++i) {
      for (int j = 0; j < num_cities; ++j) {
        if (adj_matrix[i][k] != INF && adj_matrix[k][j] != INF &&
            adj_matrix[i][k] + adj_matrix[k][j] < adj_matrix[i][j]) {
          adj_matrix[i][j] = adj_matrix[i][k] + adj_matrix[k][j];
          next_hop_matrix[i][j] = next_hop_matrix[i][k];
        }
      }
    }
  }

  std::cout << "\nGenerowanie plików DOT dla najkrótszych tras...\n";
  int files_generated = 0;
  for (int i = 0; i < num_cities; ++i) {
    for (int j = 0; j < num_cities; ++j) {
      if (i == j)
        continue;

      std::vector<int> path =
          reconstruct_path(i, j, next_hop_matrix, adj_matrix);

      if (!path.empty() && adj_matrix[i][j] != INF) {
        std::string start_city_name = sanitize_filename(id_to_city[i]);
        std::string end_city_name = sanitize_filename(id_to_city[j]);
        std::string dot_filename =
            "trasa_" + start_city_name + "_do_" + end_city_name + ".dot";
        std::string png_filename =
            "trasa_" + start_city_name + "_do_" + end_city_name + ".png";

        generate_dot_file(dot_filename, path, id_to_city, city_id_to_coords,
                          all_direct_edges_for_dot, original_direct_times_matrix);

        std::cout << "Wygenerowano: " << dot_filename << "\n";
        std::cout << "  Aby stworzyć PNG, użyj: dot -Kfdp -n -Tpng "
                  << dot_filename << " -o " << png_filename << "\n";
        files_generated++;
      }
    }
  }
  if (files_generated > 0) {
      std::cout << "\nWygenerowano " << files_generated << " plików .dot.\n";
      std::cout << "Użyj programu 'dot' (część pakietu Graphviz) do konwersji plików .dot na obrazy.\n";
      std::cout << "Przykład: dot -Kfdp -n -Tpng nazwa_pliku.dot -o nazwa_pliku.png\n";
  } else {
      std::cout << "\nNie wygenerowano żadnych plików .dot (możliwy brak tras lub miast).\n";
  }


  return 0;
}