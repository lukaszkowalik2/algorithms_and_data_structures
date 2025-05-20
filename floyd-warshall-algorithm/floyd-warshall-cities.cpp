#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

const int INF = std::numeric_limits<int>::max() / 2;

std::string truncate_name(const std::string& name, size_t max_len) {
    if (name.length() <= max_len) return name;
    return name.substr(0, max_len-2) + "..";
}

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

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  std::map<std::string, int> city_to_id;
  std::vector<std::string> id_to_city;
  int next_city_id = 0;

  std::vector<std::tuple<std::string, std::string, int>> direct_travels;

  std::ifstream input_file("czasy.txt");
  if (!input_file.is_open()) {
    std::cerr << "Błąd: Nie można otworzyć pliku czasy.txt\n";
    return 1;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    std::string city1_name, city2_name;
    int time;
    if (ss >> city1_name >> city2_name >> time) {
      direct_travels.emplace_back(city1_name, city2_name, time);
    } else {
      if (!line.empty() && (ss.fail() || ss.eof() && (city1_name.empty() || city2_name.empty()))) {
         std::cerr << "Nieprawidłowy format linii: " << line << std::endl;
      }
    }
  }
  input_file.close();

  if (direct_travels.empty()) {
    std::cout << "Brak danych o przejazdach w pliku czasy.txt." << std::endl;
    return 0;
  }

  for (const auto &travel : direct_travels) {
    get_city_id(std::get<0>(travel), city_to_id, id_to_city, next_city_id);
    get_city_id(std::get<1>(travel), city_to_id, id_to_city, next_city_id);
  }

  int num_cities = id_to_city.size();
  if (num_cities == 0) {
    std::cout << "Brak miast do przetworzenia." << std::endl;
    return 0;
  }

  std::vector<std::vector<int>> dist(num_cities,
                                     std::vector<int>(num_cities, INF));

  for (int i = 0; i < num_cities; ++i) {
    dist[i][i] = 0;
  }

  for (const auto &travel : direct_travels) {
    int u = city_to_id[std::get<0>(travel)];
    int v = city_to_id[std::get<1>(travel)];
    int time = std::get<2>(travel);
    dist[u][v] = std::min(dist[u][v], time);
    dist[v][u] = std::min(dist[v][u], time);
  }

  for (int k = 0; k < num_cities; ++k) {
    for (int i = 0; i < num_cities; ++i) {
      for (int j = 0; j < num_cities; ++j) {
        if (dist[i][k] != INF && dist[k][j] != INF) {
          if (dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
          }
        }
      }
    }
  }

  const int MAX_CITY_NAME = 12;
  const int cities_per_page = 8;

  std::cout << "\nNajkrótsze czasy przejazdu między miastami (w minutach):\n";

  for (int start = 0; start < num_cities; start += cities_per_page) {
    int end = std::min(start + cities_per_page, num_cities);
    
    std::cout << "\nZ \\ Do      ";
    for (int j = start; j < end; j++) {
      std::cout << std::setw(MAX_CITY_NAME) << std::left 
                << truncate_name(id_to_city[j], MAX_CITY_NAME) << " ";
    }
    std::cout << "\n" << std::string(11 + (end-start)*(MAX_CITY_NAME+1), '-') << "\n";

    for (int i = 0; i < num_cities; i++) {
      std::cout << std::setw(10) << std::left << truncate_name(id_to_city[i], 10) << " ";
      for (int j = start; j < end; j++) {
        if (dist[i][j] == INF) {
          std::cout << std::setw(MAX_CITY_NAME) << std::left << "---" << " ";
        } else {
          std::cout << std::setw(MAX_CITY_NAME) << std::left << dist[i][j] << " ";
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }

  return 0;
}