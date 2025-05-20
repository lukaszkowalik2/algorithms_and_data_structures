#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <vector>

static const int INF = std::numeric_limits<int>::max();
static const int NUM_ROOMS = 10;

std::map<char,int> room_to_int;
std::map<int,char> int_to_room;

using adj_t = std::vector<std::vector<std::pair<int,int>>>;

adj_t initialize_maps() {
  for(char c = 'A'; c <= 'J'; ++c) {
    int idx = c - 'A';
    room_to_int[c] = idx;
    int_to_room[idx] = c;
  }
  adj_t adj(NUM_ROOMS);

  auto add_edge = [&](char a, char b, int w){
    int u = room_to_int[a], v = room_to_int[b];
    adj[u].push_back({v,w});
    adj[v].push_back({u,w});
  };

  add_edge('A','B',3);
  add_edge('B','C',2);
  add_edge('B','D',1);
  add_edge('D','E',1);
  add_edge('E','F',1);
  add_edge('F','G',1);
  add_edge('F','H',2);
  add_edge('G','H',1);
  add_edge('C','H',2);
  add_edge('H','I',2);
  add_edge('H','J',1);


  return adj;
}

std::pair<std::vector<int>,std::vector<int>>dijkstra_with_prev(int src, const adj_t& adj) {
  std::vector<int> dist(NUM_ROOMS, INF), prev(NUM_ROOMS, -1);
  dist[src] = 0;
  using pii = std::pair<int,int>;
  std::priority_queue<pii,std::vector<pii>,
                      std::greater<pii>> pq;
  pq.push({0, src});
  while(!pq.empty()) {
    auto [d,u] = pq.top(); pq.pop();
    if(d > dist[u]) continue;
    for(auto &e: adj[u]) {
      int v = e.first, w = e.second;
      if(dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        prev[v] = u;
        pq.push({dist[v], v});
      }
    }
  }
  return {dist, prev};
}

std::vector<int> reconstruct_path(int dst, const std::vector<int>& prev) {
  std::vector<int> path;
  for(int at = dst; at != -1; at = prev[at]) {
    path.push_back(at);
  }
  std::reverse(path.begin(), path.end());
  return path;
}

int main(){
  auto adj = initialize_maps();

  std::cout << "PODAJ POKÓJ STARTOWY (A-J): ";
  char src_char, dst_char;
  std::cin >> src_char;
  src_char = std::toupper(src_char);
  std::cout << "PODAJ POKÓJ DOCELOWY (A-J): ";
  std::cin >> dst_char;
  dst_char = std::toupper(dst_char);

  if(room_to_int.count(src_char)==0 || room_to_int.count(dst_char)==0) {
    std::cerr << "Niepoprawny pokój!\n";
    return 1;
  }

  int src = room_to_int[src_char];
  int dst = room_to_int[dst_char];

  auto result = dijkstra_with_prev(src, adj);
  auto &dist = result.first;
  auto &prev = result.second;

  if(dist[dst] == INF) {
    std::cout << "Brak ścieżki " << src_char << " -> " << dst_char << "\n";
  } else {
    auto path = reconstruct_path(dst, prev);
    std::cout << "Najkrótsza ścieżka " << src_char << " -> " << dst_char << ", koszt = " << dist[dst] << "\n";
    std::cout << "Trasa: ";
    for(size_t i = 0; i < path.size(); ++i) {
      std::cout << int_to_room[path[i]];
      if(i + 1 < path.size()) std::cout << " -> ";
    }
    std::cout << "\n";
  }

  return 0;
}